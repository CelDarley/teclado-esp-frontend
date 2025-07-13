// Teste com pinos alternativos para ESP32-S3
// Pinos mais comuns e estáveis

// Pinos alternativos (mais comuns para ESP32-S3)
int rowPins[4] = {25, 26, 27, 14};  // Linhas
int colPins[3] = {12, 13, 15};       // Colunas

// Layout do teclado
char keys[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TESTE COM PINOS ALTERNATIVOS ===");
  Serial.println("Pinos configurados:");
  Serial.print("Linhas: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(rowPins[i]);
    if (i < 3) Serial.print(", ");
  }
  Serial.println();
  
  Serial.print("Colunas: ");
  for (int i = 0; i < 3; i++) {
    Serial.print(colPins[i]);
    if (i < 2) Serial.print(", ");
  }
  Serial.println();
  
  // Configurar pinos
  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  
  for (int i = 0; i < 3; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
  
  Serial.println("Sistema pronto!");
  Serial.println("Pressione qualquer tecla...");
  Serial.println("---");
}

void loop() {
  // Testar cada linha
  for (int i = 0; i < 4; i++) {
    // Ativar linha atual
    digitalWrite(rowPins[i], LOW);
    delay(10);
    
    // Verificar colunas
    for (int j = 0; j < 3; j++) {
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