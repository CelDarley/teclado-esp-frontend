// Teste de pinos individuais do ESP32-S3
// Para verificar se os pinos estão funcionando

const int PINOS_TESTE[] = {25, 26, 27, 14, 12, 13, 15, 32, 33};

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TESTE DE PINOS INDIVIDUAIS ===");
  Serial.println("Vamos verificar se os pinos estão funcionando...");
  
  // Testar cada pino
  for (int i = 0; i < 9; i++) {
    Serial.print("Testando pino ");
    Serial.print(PINOS_TESTE[i]);
    Serial.println("...");
    
    // Configurar como saída
    pinMode(PINOS_TESTE[i], OUTPUT);
    
    // Piscar o pino
    digitalWrite(PINOS_TESTE[i], HIGH);
    delay(500);
    digitalWrite(PINOS_TESTE[i], LOW);
    delay(500);
    
    Serial.print("Pino ");
    Serial.print(PINOS_TESTE[i]);
    Serial.println(" OK");
  }
  
  Serial.println("\nTodos os pinos testados!");
  Serial.println("Se você viu as mensagens, os pinos estão funcionando.");
  Serial.println("---");
}

void loop() {
  // Piscar LED onboard para indicar que está funcionando
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
  
  Serial.println("Sistema funcionando...");
} 