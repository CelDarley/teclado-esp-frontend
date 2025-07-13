// Teste específico para o teclado
// Baseado no seu mapeamento: Pino 1 + Pino 5 = Tecla "1"

// Pinos do ESP32 para conectar ao teclado
const int PIN_LINHA_1 = 25;  // Pino 1 do teclado
const int PIN_COLUNA_1 = 26; // Pino 5 do teclado

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TESTE ESPECIFICO DO TECLADO ===");
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
  delay(50); // Delay maior para estabilização
  
  // Verificar coluna
  if (digitalRead(PIN_COLUNA_1) == LOW) {
    Serial.println("TECLA 1 PRESSIONADA!");
    
    // Aguardar soltar a tecla
    while (digitalRead(PIN_COLUNA_1) == LOW) {
      delay(10);
    }
    Serial.println("Tecla solta.");
  }
  
  // Desativar linha
  digitalWrite(PIN_LINHA_1, HIGH);
  delay(50);
  
  // Debug a cada 3 segundos
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 3000) {
    Serial.println("Sistema funcionando... (aguardando tecla 1)");
    lastDebug = millis();
  }
} 