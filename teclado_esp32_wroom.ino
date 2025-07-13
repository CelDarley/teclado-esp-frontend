#include <Keypad.h>

// Configurações do teclado para ESP32-WROOM
const byte ROWS = 4;
const byte COLS = 3;

// Layout do teclado
char keys[ROWS][COLS] = {
  {'1','2','3'},  // Linha 1
  {'4','5','6'},  // Linha 2
  {'7','8','9'},  // Linha 3
  {'*','0','#'}   // Linha 4
};

// Pinos para ESP32-WROOM (pinos que funcionam)
byte rowPins[ROWS] = {4, 12, 13, 14};  // Linhas
byte colPins[COLS] = {5, 15, 16};       // Colunas

// Pinos de controle
const int LED_PIN = 2;       // LED de status
const int BUZZER_PIN = 21;   // Buzzer para feedback

// Variáveis globais
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
String currentPin = "";
unsigned long lastKeyPress = 0;
const unsigned long KEY_TIMEOUT = 10000; // 10 segundos

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TECLADO ESP32-WROOM ===");
  Serial.println("Pinos configurados para ESP32-WROOM:");
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
  
  // Configurar pinos
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Estado inicial
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  Serial.println("Sistema pronto!");
  Serial.println("Digite o PIN e pressione * para confirmar");
  Serial.println("Pressione # para cancelar");
  Serial.println("---");
  
  // Feedback inicial
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
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
        
        // Simular validação
        if (currentPin == "8729") {
          Serial.println("🎉 PIN CORRETO (Admin)!");
          showSuccess();
        } else {
          Serial.println("❌ PIN INCORRETO!");
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