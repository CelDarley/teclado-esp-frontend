#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Keypad.h>
#include "config.h"

// Configurações do teclado
const byte ROWS = 4;
const byte COLS = 3;

// Layout do teclado
char keys[ROWS][COLS] = {
  {'1','2','3'},  // Linha 1
  {'4','5','6'},  // Linha 2
  {'7','8','9'},  // Linha 3
  {'*','0','#'}   // Linha 4
};

// Pinos do ESP32 (definidos no config.h)
byte rowPins[ROWS] = ROW_PINS;
byte colPins[COLS] = COL_PINS;

// Variáveis globais
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
String currentPin = "";
unsigned long lastKeyPress = 0;
bool wifiConnected = false;
bool systemReady = false;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== SISTEMA DE CONTROLE DE ACESSO FINAL ===");
  Serial.println("ESP32 + Teclado + WiFi + Backend Django");
  Serial.println();
  
  // Configurar pinos
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  // Estado inicial
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);  // Fechadura trancada
  
  Serial.println("Pinos configurados:");
  Serial.print("Linhas: ");
  for (int i = 0; i < ROWS; i++) {
    Serial.print(rowPins[i]);
    if (i < ROWS - 1) Serial.print(", ");
  }
  Serial.println();
  
  Serial.print("Colunas: ");
  for (int i = 0; i < COLS; i++) {
    Serial.print(colPins[i]);
    if (i < COLS - 1) Serial.print(", ");
  }
  Serial.println();
  
  // Conectar ao WiFi
  connectToWiFi();
  
  // Feedback inicial
  if (wifiConnected) {
    showSystemReady();
  } else {
    showWiFiError();
  }
  
  Serial.println("Sistema pronto!");
  Serial.println("Digite o PIN e pressione * para confirmar");
  Serial.println("Pressione # para cancelar");
  Serial.println("---");
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    handleKeyPress(key);
    lastKeyPress = millis();
  }
  
  // Timeout para limpar PIN se não houver atividade
  if (currentPin.length() > 0 && (millis() - lastKeyPress) > KEY_TIMEOUT) {
    currentPin = "";
    Serial.println("⏰ TIMEOUT: PIN limpo automaticamente");
    digitalWrite(LED_PIN, LOW);
  }
  
  // Verificar conexão WiFi periodicamente
  static unsigned long lastWiFiCheck = 0;
  if (millis() - lastWiFiCheck > WIFI_CHECK_INTERVAL) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("⚠️ WiFi desconectado. Tentando reconectar...");
      wifiConnected = false;
      connectToWiFi();
    }
    lastWiFiCheck = millis();
  }
}

void connectToWiFi() {
  Serial.print("📡 Conectando ao WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println();
    Serial.print("✅ WiFi conectado! IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("📡 Servidor: ");
    Serial.println(SERVER_URL);
  } else {
    wifiConnected = false;
    Serial.println();
    Serial.println("❌ Falha na conexão WiFi!");
  }
}

void handleKeyPress(char key) {
  Serial.println();
  Serial.print("🔘 TECLA: '");
  Serial.print(key);
  Serial.println("'");
  
  switch (key) {
    case '*':
      if (currentPin.length() > 0) {
        Serial.print("✅ CONFIRMAÇÃO: PIN digitado: ");
        Serial.println(currentPin);
        Serial.print("📊 Comprimento: ");
        Serial.println(currentPin.length());
        
        if (wifiConnected) {
          verifyPinWithBackend(currentPin);
        } else {
          Serial.println("❌ ERRO: WiFi não conectado!");
          showError();
        }
        
        currentPin = "";
      } else {
        Serial.println("⚠️ ERRO: Nenhum PIN digitado para confirmar");
      }
      break;
      
    case '#':
      // Cancelar entrada
      currentPin = "";
      Serial.println("❌ CANCELADO: Entrada cancelada pelo usuário");
      digitalWrite(LED_PIN, LOW);
      break;
      
    default:
      // Adicionar dígito ao PIN
      if (currentPin.length() < 4) {
        currentPin += key;
        Serial.print("📝 PIN ATUAL: ");
        Serial.println(currentPin);
        Serial.print("📊 Dígitos: ");
        Serial.print(currentPin.length());
        Serial.println("/4");
        
        // Feedback visual
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
      } else {
        Serial.println("⚠️ ERRO: PIN já tem 4 dígitos (máximo)");
        showError();
      }
      break;
  }
  
  Serial.println("---");
}

void verifyPinWithBackend(String pin) {
  Serial.println("🌐 Enviando PIN para o backend...");
  
  HTTPClient http;
  String url = String(SERVER_URL) + String(API_ENDPOINT);
  
  Serial.print("📡 URL: ");
  Serial.println(url);
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  // Criar JSON com o PIN
  String jsonPayload = "{\"pin\":\"" + pin + "\"}";
  Serial.print("📤 Payload: ");
  Serial.println(jsonPayload);
  
  int httpResponseCode = http.POST(jsonPayload);
  
  Serial.print("📥 Código de resposta: ");
  Serial.println(httpResponseCode);
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print("📥 Resposta: ");
    Serial.println(response);
    
    // Parse da resposta JSON
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, response);
    
    if (!error) {
      bool accessGranted = doc["access_granted"];
      String message = doc["message"];
      
      Serial.print("🔓 Acesso concedido: ");
      Serial.println(accessGranted ? "SIM" : "NÃO");
      Serial.print("💬 Mensagem: ");
      Serial.println(message);
      
      if (accessGranted) {
        Serial.println("🎉 ACESSO LIBERADO!");
        unlockDoor();
        showSuccess();
      } else {
        Serial.println("❌ ACESSO NEGADO!");
        showError();
      }
    } else {
      Serial.println("❌ ERRO: Falha ao parsear resposta JSON");
      showError();
    }
  } else {
    Serial.println("❌ ERRO: Falha na requisição HTTP");
    showError();
  }
  
  http.end();
}

void unlockDoor() {
  Serial.println("🔓 LIBERANDO FECHADURA...");
  
  // Ativar relé (liberar fechadura)
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("✅ Relé ATIVADO - Fechadura liberada");
  
  // Manter aberto por 3 segundos
  delay(DOOR_OPEN_TIME);
  
  // Desativar relé (trancar fechadura)
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("🔒 Relé DESATIVADO - Fechadura trancada");
}

void showSuccess() {
  Serial.println("💚 FEEDBACK: Sucesso (LED verde + buzzer)");
  
  // LED verde (piscar 3 vezes)
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }
  
  // Buzzer de sucesso
  tone(BUZZER_PIN, 1000, 500);
}

void showError() {
  Serial.println("🔴 FEEDBACK: Erro (LED vermelho + buzzer)");
  
  // LED vermelho (piscar 5 vezes)
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
  
  // Buzzer de erro
  tone(BUZZER_PIN, 200, 1000);
}

void showSystemReady() {
  Serial.println("✅ SISTEMA PRONTO!");
  
  // LED verde fixo por 2 segundos
  digitalWrite(LED_PIN, HIGH);
  delay(2000);
  digitalWrite(LED_PIN, LOW);
  
  // Buzzer de inicialização
  tone(BUZZER_PIN, 800, 300);
}

void showWiFiError() {
  Serial.println("⚠️ SISTEMA SEM WIFI - Modo offline");
  
  // LED amarelo (piscar lentamente)
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
  
  // Buzzer de aviso
  tone(BUZZER_PIN, 400, 1000);
} 