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

// Pinos de controle
const int LED_PIN = 2;       // LED de status
const int BUZZER_PIN = 21;   // Buzzer para feedback

// Variáveis globais
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
  delay(1000); // Aguardar estabilização
  
  Serial.println("\n\n=== DEBUG DO TECLADO MATRICIAL ===");
  Serial.println("Iniciando sistema...");
  
  // Configurar pinos
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Estado inicial
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
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
  
  // Teste inicial dos pinos
  Serial.println("\n=== TESTE INICIAL DOS PINOS ===");
  testPins();
  
  Serial.println("\n=== SISTEMA PRONTO ===");
  Serial.println("Pressione qualquer tecla para testar...");
  Serial.println("---");
  
  // Feedback inicial
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // Verificar se o keypad está funcionando
  char key = keypad.getKey();
  
  if (key) {
    Serial.println();
    Serial.print("🔘 TECLA DETECTADA: '");
    Serial.print(key);
    Serial.println("'");
    
    // Mostrar posição na matriz
    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {
        if (keys[i][j] == key) {
          Serial.print("📍 Posição: Linha ");
          Serial.print(i);
          Serial.print(", Coluna ");
          Serial.println(j);
          break;
        }
      }
    }
    
    // Feedback visual e sonoro
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    
    Serial.println("---");
  }
  
  // Debug a cada 5 segundos
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 5000) {
    Serial.println("🔄 Sistema funcionando... (debug)");
    lastDebug = millis();
  }
}

void testPins() {
  Serial.println("Testando configuração dos pinos...");
  
  // Testar cada linha
  for (int i = 0; i < ROWS; i++) {
    Serial.print("Linha ");
    Serial.print(i);
    Serial.print(" (pino ");
    Serial.print(rowPins[i]);
    Serial.print("): ");
    
    // Configurar linha como saída
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], LOW);
    
    // Verificar colunas
    bool found = false;
    for (int j = 0; j < COLS; j++) {
      pinMode(colPins[j], INPUT_PULLUP);
      delay(10);
      
      if (digitalRead(colPins[j]) == LOW) {
        Serial.print("Coluna ");
        Serial.print(j);
        Serial.print(" ativa ");
        found = true;
      }
    }
    
    if (!found) {
      Serial.print("Nenhuma coluna ativa");
    }
    
    Serial.println();
    
    // Restaurar configuração
    digitalWrite(rowPins[i], HIGH);
  }
  
  Serial.println("Teste de pinos concluído.");
} 