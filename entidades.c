#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fprio.h"
#include "entidades.h"
#include "fila.h"
#include "conjunto.h"
#include "eventos.h"   

//aqui, crio as entidades

struct Heroi *cria_heroi(int ID)
{ 
    struct Heroi *Heroi = malloc(sizeof(struct Heroi));

    if(Heroi == NULL)
        return NULL;
    
    Heroi->Base_Atual = -1;
    Heroi->ID = ID;
    Heroi-> Paciencia = (rand() % (100 - 0 + 1)) + 0;
    Heroi->Velocidade = (rand() % (5000 - 50 + 1)) + 50;
    Heroi->Experiencia = 0;
    // Heroi->Base_Atual->Local.x = 0;
    // Heroi->Base_Atual->Local.y = 0; //o que colocar aqui?
    Heroi->Vida = 1;
    int num_habilidades = (rand()%(3 - 1 + 1)) + 1;
    Heroi->Habilidades = cjto_aleat(num_habilidades, 10);
    
    return Heroi;
}

struct Base *cria_base(int ID)
{
    struct Base *Base = malloc(sizeof(struct Base));
    if(!Base)
        return NULL;
    
    Base->ID = ID;
    Base->Lotacao = (rand()%(10 - 3 + 1)) + 3;
    struct fila_t *fila_espera = fila_cria(); 
    Base->Espera = fila_espera;
    Base->Presentes = cjto_cria(Base->Lotacao);
    Base->fila_max = 0;
    Base->n_missoes = 0;    
    
    // criar um conjunto de 2 números aleatórios entre 0 e tamanho do mundo
    int coordx = (rand()%(N_TAMANHO_MUNDO ));
    int coordy = (rand()%(N_TAMANHO_MUNDO ));
    Base->Local.x= coordx;
    Base->Local.y = coordy;
    return Base;
}

struct Missao *cria_missao(int ID)
{
    struct Missao *Missao = malloc(sizeof(struct Missao));
    if(!Missao)
        return NULL;

    Missao->ID = ID;
    Missao->tentativas = 0;
    // criar um conjunto de 2 números aleatórios entre 0 e tamanho do mundo
    int coordx = (rand()%(N_TAMANHO_MUNDO ));
    int coordy = (rand()%(N_TAMANHO_MUNDO ));
    Missao->Local.x = coordx;
    Missao->Local.y = coordy;
    
    // gerar um conjunto de tam aleatorio entre 6 e 10 com habilidades aleatórias
    int num_habilidades = (rand()%(10 - 6 + 1)) + 6;
    Missao->Habilidades = cjto_aleat(num_habilidades, 10);
    
    return Missao;
}

