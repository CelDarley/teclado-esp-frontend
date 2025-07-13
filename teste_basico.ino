// Teste básico do ESP32
// Sem teclado, apenas para verificar se o código executa

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=== TESTE BASICO ESP32 ===");
  Serial.println("Se você vê esta mensagem, o ESP32 está funcionando!");
  Serial.println("Iniciando sistema...");
  
  // Configurar LED
  pinMode(2, OUTPUT);
  
  Serial.println("Pinos configurados");
  Serial.println("Sistema pronto!");
  Serial.println("---");
}

void loop() {
  // Piscar LED
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
  
  // Mensagem a cada 2 segundos
  Serial.println("ESP32 funcionando... (LED piscando)");
} 