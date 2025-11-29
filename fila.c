#include <stdio.h>
#include <stdlib.h>
#include "fila.h"


struct fila_t *fila_cria ()
{
    struct fila_t *fila = malloc(sizeof(struct fila_t));
    if(!fila)
        return NULL;
    
    fila->ult = NULL;
    fila->num = 0;
    fila->prim = NULL;
    return fila;

}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi (struct fila_t *f)
{
    if(f == NULL)
        return NULL;

    struct fila_nodo_t *aux = f->prim;

    while(f->num > 0){ //faz a checagem pelo tamanho
        struct fila_nodo_t *prox = aux->prox;
        free(aux);
        aux = prox;
    }
    free(f);  
    return NULL;
    //igual fprio
}

// Insere um item no final da fila (politica FIFO).
// Retorno: 1 se tiver sucesso ou 0 se falhar.
int fila_insere (struct fila_t *f, int item)
{
    if(f == NULL)
        return 0;

    
    struct fila_nodo_t *aux = f->prim;
    while(aux != NULL){
        if(aux->item == item)
            return -1; // nao insere se o item ja ta aqui
        aux = aux->prox;
    }
    
    struct fila_nodo_t *novo_nodo = malloc(sizeof(struct fila_nodo_t)); 
    if(!novo_nodo)
        return 0;
    
    novo_nodo->item = item;
    novo_nodo->prox = NULL; 

  
    if(f->prim == NULL) 
    {
        f->prim = novo_nodo;
        f->ult = novo_nodo; 
    }
    else 
    {
     
        f->ult->prox = novo_nodo;
        
   
        f->ult = novo_nodo;
    }

  
    f->num++;

    return 1;
}
// Retira o primeiro item da fila e o devolve
// Retorno 1 se a operação foi bem sucedida e 0 caso contrário
int fila_retira (struct fila_t *f, int *item)
{
    if (!f || f->prim == NULL || item == NULL)
        return 0;

    // nodo que será removido
    struct fila_nodo_t *nodo = f->prim;

    // pega o item (id do heroi)
    *item = nodo->item;

    // atualiza a cabeça da fila
    f->prim = nodo->prox;
    
    if(f->prim == NULL)
        f->ult = NULL;

    f->num--;
    // libera apenas o nodo da fila
    free(nodo);

    return 1;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho (struct fila_t *f)
{
    if(!f || f->num < 0)
        return -1;

    if(f->num >= 0)
        return f->num;
    
    return -1;
}

// Imprime o conteúdo da fila 
void fila_imprime (struct fila_t *f)
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

    while (aux != NULL) {
        printf("%d ", aux->item);
        aux = aux->prox;
    }
    

}
