// Teste apenas da tecla 2
// Mapeamento: Pino 1 + Pino 6 = Tecla 2

const int PIN_LINHA = 1;   // Pino 1 do teclado (linha)
const int PIN_COLUNA = 6;  // Pino 6 do teclado (coluna)

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TESTE APENAS TECLA 2 ===");
  Serial.println("Mapeamento: Pino 1 + Pino 6 = Tecla 2");
  Serial.print("Linha: Pino ");
  Serial.println(PIN_LINHA);
  Serial.print("Coluna: Pino ");
  Serial.println(PIN_COLUNA);
  
  // Configurar pinos
  pinMode(PIN_LINHA, OUTPUT);
  pinMode(PIN_COLUNA, INPUT_PULLUP);
  
  Serial.println("Sistema pronto!");
  Serial.println("Pressione apenas a tecla 2...");
  Serial.println("---");
}

void loop() {
  // Ativar linha
  digitalWrite(PIN_LINHA, LOW);
  delay(50);
  
  // Verificar coluna
  if (digitalRead(PIN_COLUNA) == LOW) {
    Serial.println("TECLA 2 PRESSIONADA!");
    
    // Aguardar soltar a tecla
    while (digitalRead(PIN_COLUNA) == LOW) {
      delay(10);
    }
    Serial.println("Tecla 2 solta.");
  }
  
  // Desativar linha
  digitalWrite(PIN_LINHA, HIGH);
  delay(50);
  
  // Debug a cada 3 segundos
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 3000) {
    Serial.println("Sistema funcionando... (aguardando tecla 2)");
    lastDebug = millis();
  }
} 