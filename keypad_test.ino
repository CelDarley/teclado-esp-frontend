#include <Keypad.h>

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

// Pinos de controle para feedback
const int LED_PIN = 2;       // LED de status
const int BUZZER_PIN = 21;   // Buzzer para feedback

// Variáveis globais
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
String currentPin = "";
unsigned long lastKeyPress = 0;
const unsigned long KEY_TIMEOUT = 10000; // 10 segundos

void setup() {
  Serial.begin(115200);
  
  // Configurar pinos
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Estado inicial
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  Serial.println("=== TESTE DO TECLADO MATRICIAL ===");
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
  
  Serial.println("Layout do teclado:");
  for (int i = 0; i < ROWS; i++) {
    Serial.print("Linha ");
    Serial.print(i);
    Serial.print(": ");
    for (int j = 0; j < COLS; j++) {
      Serial.print(keys[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  
  Serial.println();
  Serial.println("=== INSTRUÇÕES ===");
  Serial.println("1. Pressione qualquer tecla para testar");
  Serial.println("2. Digite um PIN de 4 dígitos");
  Serial.println("3. Pressione * para confirmar");
  Serial.println("4. Pressione # para cancelar");
  Serial.println("5. Aguarde 10s para timeout automático");
  Serial.println();
  Serial.println("=== INICIANDO TESTE ===");
  
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
  Serial.print("🔘 TECLA PRESSIONADA: '");
  Serial.print(key);
  Serial.println("'");
  
  // Mostrar posição da tecla na matriz
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (keys[i][j] == key) {
        Serial.print("📍 Posição na matriz: Linha ");
        Serial.print(i);
        Serial.print(", Coluna ");
        Serial.println(j);
        break;
      }
    }
  }
  
  switch (key) {
    case '*':
      if (currentPin.length() > 0) {
        Serial.print("✅ CONFIRMAÇÃO: PIN digitado: ");
        Serial.println(currentPin);
        Serial.print("📊 Comprimento do PIN: ");
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
      Serial.println("❌ CANCELADO: Entrada cancelada pelo usuário");
      currentPin = "";
      digitalWrite(LED_PIN, LOW);
      break;
      
    default:
      // Adicionar dígito ao PIN
      if (currentPin.length() < 4) {
        currentPin += key;
        Serial.print("📝 PIN ATUAL: ");
        Serial.println(currentPin);
        Serial.print("📊 Dígitos digitados: ");
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