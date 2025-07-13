// Teste de uma única tecla
// Conecte apenas a tecla "1" (pinos 1 e 5 do teclado)

const int PIN_LINHA = 32;   // Pino da linha (pino 1 do teclado)
const int PIN_COLUNA = 27;  // Pino da coluna (pino 5 do teclado)

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TESTE DE UMA TECLA ===");
  Serial.println("Conecte apenas a tecla 1:");
  Serial.print("Linha: Pino ");
  Serial.println(PIN_LINHA);
  Serial.print("Coluna: Pino ");
  Serial.println(PIN_COLUNA);
  
  // Configurar pinos
  pinMode(PIN_LINHA, OUTPUT);
  pinMode(PIN_COLUNA, INPUT_PULLUP);
  
  Serial.println("Sistema pronto!");
  Serial.println("Pressione a tecla 1...");
  Serial.println("---");
}

void loop() {
  // Ativar linha
  digitalWrite(PIN_LINHA, LOW);
  delay(10);
  
  // Verificar coluna
  if (digitalRead(PIN_COLUNA) == LOW) {
    Serial.println("TECLA 1 PRESSIONADA!");
    
    // Aguardar soltar a tecla
    while (digitalRead(PIN_COLUNA) == LOW) {
      delay(10);
    }
  }
  
  // Desativar linha
  digitalWrite(PIN_LINHA, HIGH);
  delay(10);
} 