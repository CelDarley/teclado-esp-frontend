// Teste manual do teclado matricial
// Sem usar a biblioteca Keypad

// Pinos conectados às linhas e colunas
const int ROWS = 4;
const int COLS = 3;

int rowPins[ROWS] = {19, 18, 5, 17};  // Linhas
int colPins[COLS] = {16, 4, 22};       // Colunas

// Layout do teclado
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TESTE MANUAL DO TECLADO ===");
  Serial.println("Iniciando...");
  
  // Configurar pinos
  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  
  for (int i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
  
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
  
  Serial.println("Sistema pronto!");
  Serial.println("Pressione qualquer tecla...");
  Serial.println("---");
}

void loop() {
  // Testar cada linha
  for (int i = 0; i < ROWS; i++) {
    // Ativar linha atual
    digitalWrite(rowPins[i], LOW);
    delay(10);
    
    // Verificar colunas
    for (int j = 0; j < COLS; j++) {
      if (digitalRead(colPins[j]) == LOW) {
        Serial.print("TECLA DETECTADA: ");
        Serial.print(keys[i][j]);
        Serial.print(" (Linha ");
        Serial.print(i);
        Serial.print(", Coluna ");
        Serial.print(j);
        Serial.println(")");
        
        // Aguardar soltar a tecla
        while (digitalRead(colPins[j]) == LOW) {
          delay(10);
        }
      }
    }
    
    // Desativar linha atual
    digitalWrite(rowPins[i], HIGH);
  }
  
  // Debug a cada 5 segundos
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 5000) {
    Serial.println("Sistema funcionando... (testando teclas)");
    lastDebug = millis();
  }
} 