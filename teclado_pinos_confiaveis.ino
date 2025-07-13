// Teste do teclado com pinos confiáveis
// Baseado no seu mapeamento: Pino 1 + Pino 5 = Tecla "1"

// Pinos confiáveis do ESP32-S3
const int PIN_LINHA_1 = 2;   // LED onboard (funciona)
const int PIN_COLUNA_1 = 4;  // GPIO4 (estável)

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TECLADO COM PINOS CONFiAVEIS ===");
  Serial.println("Conecte o teclado:");
  Serial.print("Pino 1 do teclado -> Pino ");
  Serial.println(PIN_LINHA_1);
  Serial.print("Pino 5 do teclado -> Pino ");
  Serial.println(PIN_COLUNA_1);
  Serial.println();
  
  // Configurar pinos
  pinMode(PIN_LINHA_1, OUTPUT);
  pinMode(PIN_COLUNA_1, INPUT_PULLUP);
  
  Serial.println("Sistema pronto!");
  Serial.println("Pressione a tecla 1...");
  Serial.println("---");
}

void loop() {
  // Ativar linha
  digitalWrite(PIN_LINHA_1, LOW);
  delay(100);
  
  // Verificar coluna
  int valor = digitalRead(PIN_COLUNA_1);
  
  if (valor == LOW) {
    Serial.println("TECLA 1 PRESSIONADA!");
    
    // Aguardar soltar a tecla
    while (digitalRead(PIN_COLUNA_1) == LOW) {
      delay(10);
    }
    Serial.println("Tecla solta.");
  }
  
  // Desativar linha
  digitalWrite(PIN_LINHA_1, HIGH);
  delay(100);
  
  // Debug a cada 3 segundos
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 3000) {
    Serial.print("Valor atual: ");
    Serial.println(valor);
    lastDebug = millis();
  }
} 