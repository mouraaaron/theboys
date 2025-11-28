#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "entidades.h"


struct fila_t *fila_cria ()
{
    struct fila_t *fila = malloc(sizeof(struct fila_t));
    if(!fila)
        return NULL;
    
    fila->fim = NULL;
    fila->num = 0;
    fila->prim = NULL;
    return fila;
}

struct fila_t *fila_destroi (struct fila_t *f)
{
    struct fila_nodo_t *aux = f->prim;

    while(f->num > 0){ //faz a checagem pelo tamanho
        struct fila_nodo_t *prox = aux->prox;
        free(aux->item); //se o item foi alocado dinamicante, tem q dar free assim
        free(aux);
        f->num -=1; //diminui o tamanho
        aux = prox;
    }
    free(f);  
    return NULL;
    //igual fprio
}

int fila_insere (struct fila_t *f, void *item)
{
    // primeiro, verificar se o item já esta na fila

    struct fila_nodo_t *aux = f->prim;
    while(aux != NULL){
    if(aux->item == item)
      return -1;
    aux = aux->prox;
    }


    struct fila_nodo_t *novo_nodo = malloc(sizeof(struct fila_nodo_t)); // cria o nodo q vai ser inserido
    if(!novo_nodo)
        return -1;
    
    novo_nodo->item = item;
   
    if(f->num == 0) //caso nao tenha ninguem na fila
    {
        novo_nodo->prox = NULL;
        f->prim = novo_nodo;
        f->num += 1;
        f->fim = novo_nodo; 
    }
     
    struct fila_nodo_t *nodo_anterior = f->prim;
    while(nodo_anterior->prox != NULL)
        nodo_anterior = nodo_anterior->prox; //avança o nodo anterior
    
    nodo_anterior->prox = novo_nodo;

    f->fim = novo_nodo;
    f->num += 1;


    return f->num;
}

void *fila_retira(struct fila_t *f)
{
    if (!f || f->prim == NULL)
        return NULL;

    // nodo que será removido
    struct fila_nodo_t *nodo = f->prim;

    // pega o item (ponteiro para struct Heroi)
    void *item = nodo->item;

    // atualiza a cabeça da fila
    f->prim = nodo->prox;
    f->num--;

    // libera apenas o nodo da fila
    free(nodo);

    return item;
}

int fila_tamanho (struct fila_t *f)
{
    if(f->num >= 0)
        return f->num;
    return -1;
}

void fila_imprime(struct fila_t *f)
{
    if (!f) {
        printf("Fila NULL.\n");
        return;
    }

    struct fila_nodo_t *aux = f->prim;

    if (!aux) {
        printf("Fila vazia.\n");
        return;
    }

    while (aux) {
        printf("%p ", aux->item);
        aux = aux->prox;
    }

    printf("\n");
}

