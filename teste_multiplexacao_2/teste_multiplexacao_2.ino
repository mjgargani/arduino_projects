/*
  Matriz 3x5 (Wide) - Animação com Controle de Brilho
  Hardware: 
    - Linhas (Anodos): 7 (Verm), 6 (Am), 5 (Verde)
    - Colunas (Catodos): 12, 11, 10, 9, 8
*/

// --- Configuração de Hardware ---
int linhas[] = {7, 6, 5}; 
// Agora temos 5 pinos de colunas!
int colunas[] = {12, 11, 10, 9, 8};

// Constantes de tamanho atualizadas
const int QTD_QUADROS = 13;
const int QTD_LINHAS = 3;
const int QTD_COLUNAS = 5; // Aumentou de 3 para 5

// --- A Animação (3D Array) ---
// [Quantidade de Quadros] [Linhas] [Colunas]
// Note que agora cada linha tem 5 zeros ou uns
byte animacao[QTD_QUADROS][QTD_LINHAS][QTD_COLUNAS] = {
  // Quadro 0: Número 0
  {
    {0, 1, 1, 1, 0}, 
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
  },
  // Quadro 1: Número 1
  {
    {1, 0, 0, 1, 0}, 
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0}
  },
  // Quadro 2: Número 2
  {
    {1, 1, 0, 0, 1}, 
    {1, 0, 1, 0, 1},
    {1, 0, 0, 1, 0}
  },
  // Quadro 3: Número 3
  {
    {1, 0, 0, 0, 1}, 
    {1, 0, 1, 0, 1},
    {0, 1, 0, 1, 0}
  },
  // Quadro 4: Número 4
  {
    {0, 0, 0, 1, 1}, 
    {0, 0, 1, 0, 0},
    {1, 1, 1, 1, 1}
  },
  // Quadro 5: Número 5
  {
    {1, 0, 0, 1, 1}, 
    {1, 0, 1, 0, 1},
    {0, 1, 0, 0, 1}
  },
  // Quadro 6: Número 6
  {
    {0, 1, 1, 1, 0}, 
    {1, 0, 1, 0, 1},
    {0, 1, 0, 0, 0}
  },
  // Quadro 7: Número 7
  {
    {1, 1, 0, 0, 1}, 
    {0, 0, 1, 0, 1},
    {0, 0, 0, 1, 1}
  },
  // Quadro 8: Número 8
  {
    {1, 1, 0, 1, 1}, 
    {1, 0, 1, 0, 1},
    {1, 1, 0, 1, 1}
  },
  // Quadro 9: Número 9
  {
    {0, 0, 0, 1, 0}, 
    {1, 0, 1, 0, 1},
    {0, 1, 1, 1, 0}
  },
  // Quadro 10: Quadrado
  {
    {0, 0, 0, 0, 0}, 
    {0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0}
  },
  // Quadro 11: Retângulo Invertido
  {
    {0, 0, 1, 0, 0}, 
    {0, 1, 0, 1, 0},
    {0, 0, 1, 0, 0}
  },
  // Quadro 12: Quadrado Invertido
  {
    {1, 0, 1, 0, 1}, 
    {0, 1, 0, 1, 0},
    {1, 0, 1, 0, 1}
  }
};

void setup() {
  // Configura pinos das LINHAS (3 pinos)
  for (int i = 0; i < QTD_LINHAS; i++) {
    pinMode(linhas[i], OUTPUT);
    digitalWrite(linhas[i], LOW); 
  }
  // Configura pinos das COLUNAS (5 pinos)
  for (int i = 0; i < QTD_COLUNAS; i++) {
    pinMode(colunas[i], OUTPUT);
    digitalWrite(colunas[i], HIGH); // Catodo HIGH = Off
  }
}

void loop() {
  for (int k = 0; k < QTD_QUADROS; k++) {
    // 1. Mostra imagem com brilho total
    exibirPorTempo(k, 100, 600);

    // 2. Efeito Fade Out (Apaga suavemente)
    for (int b = 100; b >= 0; b -= 4) { // Aumentei o passo para ser mais rápido
      exibirPorTempo(k, b, 15); 
    }
    
    // Pequena pausa escura entre números
    delay(100);
  }
}

// --- ENGINE GRÁFICA (Adaptada para 5 Colunas) ---
void exibirPorTempo(int quadroIndex, int brilho, int duracao) {
  
  unsigned long inicio = millis();
  
  // Ajuste do ciclo para 5 colunas (mantendo fluidez)
  int tempoCicloPorLinha = 2000; 
  int tempoLigado = map(brilho, 0, 100, 0, tempoCicloPorLinha);
  int tempoDesligado = tempoCicloPorLinha - tempoLigado;

  while (millis() - inicio < duracao) {
    
    // Varredura das LINHAS
    for (int i = 0; i < QTD_LINHAS; i++) {
      
      // 1. Configura as 5 COLUNAS para a linha atual
      for (int j = 0; j < QTD_COLUNAS; j++) {
        if (animacao[quadroIndex][i][j] == 1) {
          digitalWrite(colunas[j], LOW); // Liga pixel (GND)
        } else {
          digitalWrite(colunas[j], HIGH); // Desliga pixel (VCC)
        }
      }

      // 2. Acende a Linha (PWM ON)
      if (tempoLigado > 0) {
        digitalWrite(linhas[i], HIGH);
        delayMicroseconds(tempoLigado);
      }

      // 3. Apaga a Linha (PWM OFF)
      digitalWrite(linhas[i], LOW);
      if (tempoDesligado > 0) {
        delayMicroseconds(tempoDesligado);
      }
      
      // 4. Limpa as 5 colunas
      for (int k = 0; k < QTD_COLUNAS; k++) digitalWrite(colunas[k], HIGH);
    }
  }
}
