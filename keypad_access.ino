#include <Keypad.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Configurações WiFi
const char* ssid = "SUA_REDE_WIFI";
const char* password = "SUA_SENHA_WIFI";

// Configurações do servidor
const char* serverUrl = "http://localhost:8000/api/access/";

// Configurações do teclado
const byte ROWS = 4;
const byte COLS = 3;

// Layout do teclado
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Pinos conectados às linhas e colunas
byte rowPins[ROWS] = {19, 18, 5, 17};  // Linhas
byte colPins[COLS] = {16, 4, 22};       // Colunas

// Pinos de controle
const int RELAY_PIN = 23;    // Relé para fechadura magnética
const int LED_PIN = 2;       // LED de status
const int BUZZER_PIN = 21;   // Buzzer para feedback

// Variáveis globais
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
String currentPin = "";
bool isAdminMode = false;
unsigned long lastKeyPress = 0;
const unsigned long KEY_TIMEOUT = 10000; // 10 segundos

void setup() {
  Serial.begin(115200);
  
  // Configurar pinos
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Estado inicial
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  Serial.println("=== SISTEMA DE CONTROLE DE ACESSO ===");
  Serial.println("Configuração do teclado:");
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
  
  // Conectar WiFi
  WiFi.begin(ssid, password);
  Serial.println("Conectando ao WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  
  // Feedback inicial
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("Sistema de controle de acesso iniciado!");
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
        validatePin(currentPin);
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

void validatePin(String pin) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ ERRO: WiFi não conectado");
    showError();
    return;
  }
  
  Serial.println("🌐 Enviando PIN para o servidor...");
  
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  
  // Criar JSON para envio
  String jsonData = "{\"pin\":\"" + pin + "\"}";
  Serial.print("📤 JSON enviado: ");
  Serial.println(jsonData);
  
  int httpResponseCode = http.POST(jsonData);
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print("📥 Resposta do servidor (");
    Serial.print(httpResponseCode);
    Serial.print("): ");
    Serial.println(response);
    
    // Parsear JSON
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, response);
    
    if (doc["success"] == true) {
      Serial.println("🎉 ACESSO AUTORIZADO!");
      showSuccess();
      openDoor(doc["duration"] | 5);
    } else {
      Serial.println("❌ ACESSO NEGADO!");
      showError();
    }
  } else {
    Serial.print("❌ ERRO HTTP: ");
    Serial.println(httpResponseCode);
    showError();
  }
  
  http.end();
}

void openDoor(int duration) {
  Serial.print("🔓 ABRINDO PORTA por ");
  Serial.print(duration);
  Serial.println(" segundos");
  
  // Ativar relé
  digitalWrite(RELAY_PIN, HIGH);
  
  // Feedback visual e sonoro
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
  
  // Manter porta aberta
  delay(duration * 1000);
  
  // Desativar relé
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("🔒 PORTA FECHADA");
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