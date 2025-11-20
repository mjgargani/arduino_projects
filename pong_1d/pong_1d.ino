/*
  PONG 7 LEDs - Versão Final (Alternada + Auto-Restart)
  - Critério: Clique RÁPIDO quando o LED da ponta acender.
  - Anti-Cheat: Segurar o botão não funciona.
  - Reinício: Automático após vitória/derrota.
*/

// --- Pinos ---
const int pinoBtnEsq = 3;
const int pinoBtnDir = 2;
const int pinoPontoEsq = 5; // Placar Esquerda
const int pinoPontoDir = 4; // Placar Direita
// Array do Campo: Esq(13) -> ... -> Dir(7)
int leds[] = {12, 11, 10, 9, 8, 7, 6}; 
int qtdLeds = 7;

// --- Variáveis Globais ---
int posicaoBola = 3;      
int direcao = 1;          
int velocidadeInicial = 400; // Velocidade base
int velocidade = velocidadeInicial;

unsigned long tempoUltimoMovimento = 0;
bool jogoRodando = true;
int proximaDirecaoInicio = 1; // 1 = Começa indo pra Direita, -1 = Esquerda

// --- Anti-Cheat ---
bool apertouEsq = false;
bool apertouDir = false;
int lastStateEsq = HIGH;
int lastStateDir = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(pinoBtnEsq, INPUT_PULLUP);
  pinMode(pinoBtnDir, INPUT_PULLUP);
  pinMode(pinoPontoEsq, OUTPUT);
  pinMode(pinoPontoDir, OUTPUT);
  for (int i = 0; i < qtdLeds; i++) pinMode(leds[i], OUTPUT);
  
  iniciarRodada();
}

void loop() {
  // Se o jogo parou (game over), não faz nada até o reset automático acontecer
  if (!jogoRodando) return; 

  // --- 1. LEITURA DOS BOTÕES (Anti-Cheat / Borda de Descida) ---
  int leituraEsq = digitalRead(pinoBtnEsq);
  int leituraDir = digitalRead(pinoBtnDir);

  // Detecta o CLIQUE (Transição de Solto para Apertado)
  if (lastStateEsq == HIGH && leituraEsq == LOW) {
    apertouEsq = true;
  }
  if (lastStateDir == HIGH && leituraDir == LOW) {
    apertouDir = true;
  }
  lastStateEsq = leituraEsq;
  lastStateDir = leituraDir;

  // --- 2. MOVIMENTO DA BOLA ---
  if (millis() - tempoUltimoMovimento > velocidade) {
    tempoUltimoMovimento = millis();
    moverBola();
    
    // Limpa a memória de clique APÓS o movimento
    // Isso obriga o jogador a clicar novamente na hora certa
    apertouEsq = false;
    apertouDir = false;
  }
}

void iniciarRodada() {
  // Limpa tudo
  for(int i=0; i<qtdLeds; i++) digitalWrite(leds[i], LOW);
  digitalWrite(pinoPontoEsq, LOW);
  digitalWrite(pinoPontoDir, LOW);
  
  // Reseta variáveis
  posicaoBola = 3; // Meio (Pino 10)
  direcao = proximaDirecaoInicio; // Usa a direção alternada
  velocidade = velocidadeInicial;
  jogoRodando = true;
  apertouEsq = false;
  apertouDir = false;

  // Animação de início (Pisca o meio 3x indicando para onde vai)
  for(int k=0; k<3; k++) {
    digitalWrite(leds[3], HIGH); // Acende meio
    // Pequena dica visual: acende levemente o próximo LED da direção
    if(direcao == 1) digitalWrite(leds[4], HIGH); 
    else digitalWrite(leds[2], HIGH);
    
    delay(200);
    digitalWrite(leds[3], LOW);
    digitalWrite(leds[4], LOW); 
    digitalWrite(leds[2], LOW);
    delay(200);
  }
  
  // Acende o meio e começa o relógio
  digitalWrite(leds[posicaoBola], HIGH);
  tempoUltimoMovimento = millis();
}

void moverBola() {
  digitalWrite(leds[posicaoBola], LOW); // Apaga LED atual

  // --- Colisão Ponta ESQUERDA (Índice 0 / Pino 13) ---
  if (posicaoBola == 0 && direcao == -1) {
    if (apertouEsq) { 
      rebater();
      return; 
    } else {
      finalizarRodada(pinoPontoDir); // Ponto para Direita
      return;
    }
  }

  // --- Colisão Ponta DIREITA (Índice 6 / Pino 7) ---
  else if (posicaoBola == (qtdLeds - 1) && direcao == 1) {
    if (apertouDir) { 
      rebater();
      return; 
    } else {
      finalizarRodada(pinoPontoEsq); // Ponto para Esquerda
      return;
    }
  }

  // Move normal
  posicaoBola += direcao;
  digitalWrite(leds[posicaoBola], HIGH);
}

void rebater() {
  direcao *= -1; // Inverte
  posicaoBola += direcao; 
  digitalWrite(leds[posicaoBola], HIGH);
  velocidade = velocidade * 0.85; // Acelera
}

void finalizarRodada(int pinoVencedor) {
  jogoRodando = false; // Pausa o loop principal
  
  // Animação de Gol
  for(int i=0; i<5; i++) {
    digitalWrite(pinoVencedor, HIGH); // Placar pisca
    // Pisca todos os LEDs do campo
    for(int j=0; j<qtdLeds; j++) digitalWrite(leds[j], HIGH);
    delay(150);
    digitalWrite(pinoVencedor, LOW);
    for(int j=0; j<qtdLeds; j++) digitalWrite(leds[j], LOW);
    delay(150);
  }
  
  // Deixa o placar aceso por 2 segundos para ver quem ganhou
  digitalWrite(pinoVencedor, HIGH);
  delay(2000);
  
  // ALTERNA O INÍCIO para a próxima rodada
  proximaDirecaoInicio *= -1; 
  
  // Recomeça automaticamente
  iniciarRodada();
}
