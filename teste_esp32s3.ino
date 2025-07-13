// Teste específico para ESP32-S3-WROOM-1
// Configurações otimizadas para esta placa

void setup() {
  // Inicializar Serial com delay para estabilização
  Serial.begin(115200);
  delay(2000); // Aguardar estabilização do ESP32-S3
  
  Serial.println("\n\n=== TESTE ESP32-S3-WROOM-1 ===");
  Serial.println("Iniciando sistema...");
  
  // Configurar LED onboard (pino 2)
  pinMode(2, OUTPUT);
  
  Serial.println("Pinos configurados");
  Serial.println("Sistema pronto!");
  Serial.println("---");
  
  // Feedback inicial
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
}

void loop() {
  // Piscar LED a cada 2 segundos
  digitalWrite(2, HIGH);
  delay(2000);
  digitalWrite(2, LOW);
  delay(2000);
  
  // Mensagem de debug
  Serial.println("ESP32-S3 funcionando... (LED piscando a cada 2s)");
} 