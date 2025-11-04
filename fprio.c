#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fprio.h"


struct fprio_t *fprio_cria (){
    struct fprio_t *fila = malloc(sizeof(struct fprio_t));
    if(!fila)
        return NULL;
    
    memset(fila, 0, sizeof(struct fprio_t)); //settar todos os campos da fila para 0
    return fila; 
    // igual do tp5
}

struct fprio_t *fprio_destroi (struct fprio_t *f){
  if(!f)
    return NULL;

  struct fpnodo_t *aux = f->prim;

  while(f->num > 0){ //faz a checagem pelo tamanho
    struct fpnodo_t *prox = aux->prox;
    free(aux->item); //se o item foi alocado dinamicante, tem q dar free assim
    free(aux);
    f->num -=1; //diminui o tamanho
    aux = prox;
    }
  free(f);
  return NULL;
  // igual do tp5
}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio){

  if(!f || !item) //tem que tratar item == NULL como erro
    return -1;

  struct fpnodo_t *aux = f->prim;

  while(aux != NULL){
    if(aux->item == item)
      return -1;
    aux = aux->prox;
  } //tem que verificar se o item já está na lista. se estiver é erro

  struct fpnodo_t *nodo_inserido = malloc(sizeof(struct fpnodo_t));

  if(!nodo_inserido)
    return -1;

  nodo_inserido->item = item;
  nodo_inserido->tipo = tipo;
  nodo_inserido->prio = prio;
  nodo_inserido->prox = NULL;
  
 /* se o item tiver maior pridade que os que já estão na fila ou não há elementos na fila ele é inserido na primeira posição */ 
  if(f->num == 0 || prio < f->prim->prio){
    nodo_inserido->prox = f->prim;
    f->prim = nodo_inserido;
    if(f->num == 0)
        f->fim = nodo_inserido; //no the boys temos o ponteiro de fim. Temos que abordar esse caso.
    f->num +=1;
    return f->num;
  }

  aux = f->prim;
  struct fpnodo_t *anterior_aux = NULL;

  while(aux != NULL && aux->prio <= prio){ //usado para percorrer a lista encadeada. respeitando as politicas FIFO
    anterior_aux = aux; //avança o de trás
    aux = aux->prox; //avança o da frente
  }
  // agora tem que colocar o nodo insrido entre o aux e o anterior de aux
  anterior_aux->prox = nodo_inserido;
  nodo_inserido->prox = aux; 

  // como no the boys temos o campo fim na fila, temos que abordar o caso de o nodo inserido ser o útimo.
  // nesse caso, o ponteiro de fim aponta para ele
  if(nodo_inserido->prox == NULL)
    f->fim = nodo_inserido;


  f->num +=1;
  return f->num;
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio){
  if (f == NULL)
    return NULL;
  if (f->prim == NULL)
    return NULL;
  if (tipo == NULL)
    return NULL;
  if (prio == NULL)
    return NULL;
  
  struct fpnodo_t *nodo_retirado = f->prim;
  f->prim = f->prim->prox;
  f->num -= 1;
  
  if(tipo != NULL)
    *tipo = nodo_retirado->tipo;

  if(prio != NULL)
    *prio = nodo_retirado->prio;

  void *aux = nodo_retirado->item;  

  free(nodo_retirado);
  return(aux);
// copiado do tp5
}

int fprio_tamanho (struct fprio_t *f){
  if (!f)
    return -1;
  else
    return f->num;
//copiado do tp5
}

void fprio_imprime (struct fprio_t *f){
  if(!f)
    return;

  struct fpnodo_t *aux = f->prim;
  while(aux != NULL){
    printf("(%d %d)", aux->tipo, aux->prio);

    if(aux->prox != NULL)
      printf(" "); //para a última linha das especificações da função

    aux = aux->prox;
  }
//copiado do tp5
}
