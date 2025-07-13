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

// Variáveis globais
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
  delay(2000); // Aguardar estabilização
  
  Serial.println("\n\n=== TESTE SIMPLES DO TECLADO ===");
  Serial.println("Iniciando...");
  
  // Teste básico dos pinos
  Serial.println("Testando pinos...");
  for (int i = 0; i < ROWS; i++) {
    Serial.print("Linha ");
    Serial.print(i);
    Serial.print(" (pino ");
    Serial.print(rowPins[i]);
    Serial.println(")");
  }
  
  for (int i = 0; i < COLS; i++) {
    Serial.print("Coluna ");
    Serial.print(i);
    Serial.print(" (pino ");
    Serial.print(colPins[i]);
    Serial.println(")");
  }
  
  Serial.println("Sistema pronto!");
  Serial.println("Pressione qualquer tecla...");
  Serial.println("---");
}

void loop() {
  // Verificar se o keypad está funcionando
  char key = keypad.getKey();
  
  if (key) {
    Serial.print("TECLA PRESSIONADA: ");
    Serial.println(key);
    
    // Mostrar posição na matriz
    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {
        if (keys[i][j] == key) {
          Serial.print("Posição: Linha ");
          Serial.print(i);
          Serial.print(", Coluna ");
          Serial.println(j);
          break;
        }
      }
    }
    Serial.println("---");
  }
  
  // Debug a cada 3 segundos
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 3000) {
    Serial.println("Sistema funcionando... (aguardando tecla)");
    lastDebug = millis();
  }
} 