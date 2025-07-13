// Teste de conexão simples
// Conecte um fio entre pino 25 e pino 26

const int PIN_SAIDA = 25;   // Pino que vai enviar sinal
const int PIN_ENTRADA = 26; // Pino que vai receber sinal

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TESTE DE CONEXAO SIMPLES ===");
  Serial.println("Conecte um fio entre pino 25 e pino 26");
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
  if (digitalRead(PIN_ENTRADA) == LOW) {
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