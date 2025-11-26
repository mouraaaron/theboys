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
void eventos_iniciais(struct Mundo *Mundo)
{
  
  for(int i = 0; i < Mundo->NHerois; i++)
  {
    // coloca herois na lef 
    struct Evento *evento = malloc(sizeof(struct Evento));
    evento->Tempo = (rand() % 4320 - 0) + 0;
    evento->ID_Base = (rand() % Mundo->NBases - 0) + 0;
    evento->Tipo = EV_Chega;
    
    fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);

  }

  //colocando as missoes nA LEF
  for(int u = 0; u < Mundo->NMissoes; u++)
  {

    struct Evento *ev = malloc(sizeof(struct Evento));
    ev->Tempo = (rand() % T_FIM_DO_MUNDO);
    ev->Tipo = EV_Missao;
    ev->ID_Missao = Mundo->Missoes[u]->ID;
    
    fprio_insere(Mundo->LEF, ev, ev->Tipo, ev->Tempo);
  }

  //settando o fim
  struct Evento *ev_fim = malloc(sizeof(struct Evento));
  ev_fim->Tipo = EV_Fim;
  ev_fim->Tempo = T_FIM_DO_MUNDO;
  
  fprio_insere(Mundo->LEF, ev_fim, ev_fim->Tipo, ev_fim->Tempo);
}

void destroi_mundo(struct Mundo *Mundo)
{
  if(Mundo == NULL)
    return;

  //vamos destruir as bases
  for(int i = 0; i < Mundo->NBases; i++)
  {
    fila_destroi(Mundo->Bases[i]->Espera);
    cjto_destroi(Mundo->Bases[i]->Presentes);
    free(Mundo->Bases[i]);
  }
  free(Mundo->Bases);

  // vamos destruir herois
  for(int u = 0; u < Mundo->NHerois; u++)
  {
    cjto_destroi(Mundo->Herois_vivos[u]->Habilidades);
    free(Mundo->Herois_vivos[u]);
  }
  free(Mundo->Herois_vivos);

  //vamos destruir as missoes
  for(int w = 0; w < Mundo->NMissoes; w++)
  {
    cjto_destroi(Mundo->Missoes[w]->Habilidades);
    free(Mundo->Missoes[w]);
  }
  free(Mundo->Missoes);


  fprio_destroi(Mundo->LEF);
  free(Mundo);
}
// programa principal
int main ()
{

  srand(time(NULL));

  struct Mundo *M = cria_mundo();
  if(!M)
  {
    fprintf(stderr, "Erro ao criar o mundo no main\n");
    exit(1);
  }
  
  void eventos_iniciais(M);

 // incia_lef(M);

  // executar o laço de simulação

  // destruir o mundo
  destroi_mundo(M);

  return (0) ;
}

