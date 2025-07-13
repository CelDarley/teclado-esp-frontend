// Teste da tecla 2 com pinos diferentes
// Usando pinos que sabemos que funcionam no ESP32-S3

const int PIN_LINHA = 25;   // Pino 25 do ESP32 (linha)
const int PIN_COLUNA = 26;  // Pino 26 do ESP32 (coluna)

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TESTE TECLA 2 COM PINOS DIFERENTES ===");
  Serial.println("Conecte:");
  Serial.print("Pino 1 do teclado -> Pino ");
  Serial.println(PIN_LINHA);
  Serial.print("Pino 6 do teclado -> Pino ");
  Serial.println(PIN_COLUNA);
  
  // Configurar pinos
  pinMode(PIN_LINHA, OUTPUT);
  pinMode(PIN_COLUNA, INPUT_PULLUP);
  
  Serial.println("Sistema pronto!");
  Serial.println("Pressione a tecla 2...");
  Serial.println("---");
}

void loop() {
  // Ativar linha
  digitalWrite(PIN_LINHA, LOW);
  delay(100);
  
  // Verificar coluna
  int valor = digitalRead(PIN_COLUNA);
  
  if (valor == LOW) {
    Serial.println("TECLA 2 PRESSIONADA!");
    
    // Aguardar soltar a tecla
    while (digitalRead(PIN_COLUNA) == LOW) {
      delay(10);
    }
    Serial.println("Tecla 2 solta.");
  }
  
  // Desativar linha
  digitalWrite(PIN_LINHA, HIGH);
  delay(100);
  
  // Debug a cada 2 segundos
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 2000) {
    Serial.print("Valor lido: ");
    Serial.println(valor);
    lastDebug = millis();
  }
} 