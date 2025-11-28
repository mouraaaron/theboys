// programa principal do projeto "The Boys - 2024/2"
// Autor: Aaron Moura, GRR 20255804

// seus #includes vão aqui
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "fila.h"
#include "conjunto.h"
#include "entidades.h"
#include "eventos.h"
#include "lef.h"
#include "fprio.h" 
#include "lef.h"




// seus #defines vão aqui
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)
#define N_COMPOSTOS_V (N_HABILIDADES * 3)

// minimize o uso de variáveis globais

// programa principal
int main ()
{

  srand(time(NULL));

  struct Mundo *Mundo = cria_mundo();
  if(!Mundo)
  {
    fprintf(stderr, "Erro ao criar o mundo no main\n");
    exit(1);
  }
  
  eventos_iniciais(Mundo);

  // executar o laço de simulação
  inicia_lef(Mundo);
  
  // destruir o mundo
  destroi_mundo(Mundo);

  return (0) ;
}

