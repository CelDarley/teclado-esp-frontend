// Teste alternativo para o teclado
// Configuração diferente: sem pull-up

const int PIN_LINHA_1 = 25;  // Pino 1 do teclado
const int PIN_COLUNA_1 = 26; // Pino 5 do teclado

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TESTE ALTERNATIVO DO TECLADO ===");
  Serial.println("Configuração sem pull-up");
  Serial.print("Pino 1 do teclado -> Pino ");
  Serial.println(PIN_LINHA_1);
  Serial.print("Pino 5 do teclado -> Pino ");
  Serial.println(PIN_COLUNA_1);
  Serial.println();
  
  // Configurar pinos (sem pull-up)
  pinMode(PIN_LINHA_1, OUTPUT);
  pinMode(PIN_COLUNA_1, INPUT); // Sem pull-up
  
  Serial.println("Sistema pronto!");
  Serial.println("Pressione a tecla 1...");
  Serial.println("---");
}

void loop() {
  // Ativar linha
  digitalWrite(PIN_LINHA_1, LOW);
  delay(100); // Delay maior
  
  // Verificar coluna
  int valor = digitalRead(PIN_COLUNA_1);
  Serial.print("Valor lido: ");
  Serial.println(valor);
  
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
  
  // Debug a cada 2 segundos
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 2000) {
    Serial.println("Sistema funcionando...");
    lastDebug = millis();
  }
} 