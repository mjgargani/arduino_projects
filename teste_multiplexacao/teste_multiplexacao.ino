/*
  Teste de Multiplexação: Cima vs Baixo
  Canal A (Cima): Pino 13
  Canal B (Baixo): Pino 12
*/

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);
  Serial.println("Inicio do Teste POV...");
}

void loop() {
  // Loop de Aceleração:
  // Começa lento (200ms) e vai até a velocidade da luz (3ms)
  
  for (int tempo = 200; tempo > 2; tempo--) {
    
    // Repete o ciclo algumas vezes na mesma velocidade para o olho acostumar
    for (int k = 0; k < 5; k++) {
      
      // --- FRAME 1: Mostra CIMA ---
      digitalWrite(13, HIGH); // Liga Cima
      digitalWrite(12, LOW);  // Desliga Baixo
      delay(tempo);           

      // --- FRAME 2: Mostra BAIXO ---
      digitalWrite(13, LOW);  // Desliga Cima
      digitalWrite(12, HIGH); // Liga Baixo
      delay(tempo);           
    }
    
    // Debug: Mostra a velocidade atual no PC a cada 20ms
    if (tempo % 20 == 0) {
      Serial.print("Velocidade de troca: ");
      Serial.print(tempo);
      Serial.println(" ms");
    }
  }
  
  // --- MODO ILUSÃO MÁXIMA (Loop Infinito) ---
  Serial.println("--- ILUSAO ATIVADA (3ms) ---");
  while (true) {
      // Varredura ultra-rápida
      digitalWrite(13, HIGH); digitalWrite(12, LOW); delay(3);
      digitalWrite(13, LOW); digitalWrite(12, HIGH); delay(3);
  }
}
