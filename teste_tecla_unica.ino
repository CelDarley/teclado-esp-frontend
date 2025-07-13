// Teste de uma única tecla
// Substitua PIN_LINHA e PIN_COLUNA pelos pinos que você descobriu

const int PIN_LINHA = 25;   // Substitua pelo pino da linha
const int PIN_COLUNA = 12;  // Substitua pelo pino da coluna

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n=== TESTE DE UMA TECLA ===");
  Serial.print("Linha: ");
  Serial.println(PIN_LINHA);
  Serial.print("Coluna: ");
  Serial.println(PIN_COLUNA);
  
  // Configurar pinos
  pinMode(PIN_LINHA, OUTPUT);
  pinMode(PIN_COLUNA, INPUT_PULLUP);
  
  Serial.println("Sistema pronto!");
  Serial.println("Pressione a tecla conectada...");
  Serial.println("---");
}

void loop() {
  // Ativar linha
  digitalWrite(PIN_LINHA, LOW);
  delay(10);
  
  // Verificar coluna
  if (digitalRead(PIN_COLUNA) == LOW) {
    Serial.println("TECLA PRESSIONADA!");
    
    // Aguardar soltar a tecla
    while (digitalRead(PIN_COLUNA) == LOW) {
      delay(10);
    }
  }
  
  // Desativar linha
  digitalWrite(PIN_LINHA, HIGH);
  delay(10);
} 