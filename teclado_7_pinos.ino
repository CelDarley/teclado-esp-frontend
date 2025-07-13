// Código específico para teclado com 7 pinos
// Baseado no padrão: Pino 1 (linha) + Pinos 5,6,7 (colunas)

// Pinos do teclado (baseado no seu teste)
const int PINOS_LINHAS[] = {1, 2, 3, 4};  // Linhas
const int PINOS_COLUNAS[] = {5, 6, 7};     // Colunas

// Layout do teclado (baseado no seu teste)
char teclas[4][3] = {
  {'1','2','3'},  // Linha 1 (Pino 1)
  {'4','5','6'},  // Linha 2 (Pino 2)
  {'7','8','9'},  // Linha 3 (Pino 3)
  {'*','0','#'}   // Linha 4 (Pino 4)
};

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TECLADO 7 PINOS ===");
  Serial.println("Configuração baseada no seu teste:");
  Serial.println("Pino 1 + Pino 5 = Tecla 1");
  Serial.println("Pino 1 + Pino 6 = Tecla 2");
  Serial.println("Pino 1 + Pino 7 = Tecla 7");
  Serial.println();
  
  // Configurar pinos das linhas
  for (int i = 0; i < 4; i++) {
    pinMode(PINOS_LINHAS[i], OUTPUT);
    digitalWrite(PINOS_LINHAS[i], HIGH);
  }
  
  // Configurar pinos das colunas
  for (int i = 0; i < 3; i++) {
    pinMode(PINOS_COLUNAS[i], INPUT_PULLUP);
  }
  
  Serial.println("Pinos configurados:");
  Serial.print("Linhas: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(PINOS_LINHAS[i]);
    if (i < 3) Serial.print(", ");
  }
  Serial.println();
  
  Serial.print("Colunas: ");
  for (int i = 0; i < 3; i++) {
    Serial.print(PINOS_COLUNAS[i]);
    if (i < 2) Serial.print(", ");
  }
  Serial.println();
  
  Serial.println("Sistema pronto!");
  Serial.println("Pressione qualquer tecla...");
  Serial.println("---");
}

void loop() {
  // Testar cada linha
  for (int linha = 0; linha < 4; linha++) {
    // Ativar linha atual
    digitalWrite(PINOS_LINHAS[linha], LOW);
    delay(10);
    
    // Verificar colunas
    for (int coluna = 0; coluna < 3; coluna++) {
      if (digitalRead(PINOS_COLUNAS[coluna]) == LOW) {
        char tecla = teclas[linha][coluna];
        Serial.print("TECLA PRESSIONADA: ");
        Serial.print(tecla);
        Serial.print(" (Linha ");
        Serial.print(linha + 1);
        Serial.print(", Coluna ");
        Serial.print(coluna + 1);
        Serial.print(") - Pinos ");
        Serial.print(PINOS_LINHAS[linha]);
        Serial.print(" + ");
        Serial.println(PINOS_COLUNAS[coluna]);
        
        // Aguardar soltar a tecla
        while (digitalRead(PINOS_COLUNAS[coluna]) == LOW) {
          delay(10);
        }
      }
    }
    
    // Desativar linha atual
    digitalWrite(PINOS_LINHAS[linha], HIGH);
  }
  
  // Debug a cada 5 segundos
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 5000) {
    Serial.println("Sistema funcionando... (testando teclas)");
    lastDebug = millis();
  }
} 