// Teste de conexão com pinos diferentes
// Pinos mais confiáveis para ESP32-S3

const int PIN_SAIDA = 2;    // LED onboard (sabemos que funciona)
const int PIN_ENTRADA = 4;  // Pino GPIO4 (mais estável)

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TESTE CONEXAO COM PINOS DIFERENTES ===");
  Serial.println("Conecte um fio entre pino 2 (LED) e pino 4");
  Serial.print("Saída: Pino ");
  Serial.println(PIN_SAIDA);
  Serial.print("Entrada: Pino ");
  Serial.println(PIN_ENTRADA);
  
  // Configurar pinos
  pinMode(PIN_SAIDA, OUTPUT);
  pinMode(PIN_ENTRADA, INPUT_PULLUP);
  
  Serial.println("Sistema pronto!");
  Serial.println("Conecte o fio e veja se detecta...");
  Serial.println("---");
}

void loop() {
  // Enviar sinal
  digitalWrite(PIN_SAIDA, LOW);
  delay(100);
  
  // Verificar se recebeu
  int valor = digitalRead(PIN_ENTRADA);
  Serial.print("Valor lido: ");
  Serial.println(valor);
  
  if (valor == LOW) {
    Serial.println("CONEXAO DETECTADA!");
  }
  
  // Desligar
  digitalWrite(PIN_SAIDA, HIGH);
  delay(100);
  
  // Debug a cada 2 segundos
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 2000) {
    Serial.println("Testando conexão...");
    lastDebug = millis();
  }
} 