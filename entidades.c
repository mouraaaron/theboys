#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "entidades.h"
#include "fprio.h"
#include "conjunto.h"


struct Heroi *cria_heroi(int ID)
{ 
    struct Heroi *Heroi = malloc(sizeof(struct Heroi));

    if(Heroi == NULL)
        return NULL;
    
    Heroi->ID = ID;
    Heroi-> Paciencia = (rand() % (100 - 0 + 1)) + 0;
    Heroi->Velocidade = (rand() % (5000 - 500 + 1)) + 50;
    Heroi->Experiencia = 0;
    Heroi->Base_Atual = 0; //o que colocar aqui?
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
    Base->Lotacao = (rand()%(8 - 3 + 1)) + 3;
    struct fila_t *fila_espera = fila_cria(); 
    Base->Espera = fila_espera;
    Base->Presentes = cjto_cria(Base->Lotacao);

    // criar um conjunto de 2 números aleatórios entre 0 e tamanho do mundo
    int coordx = (rand()%(N_TAMANHO_MUNDO - 0 + 1)) + 0;
    int coordy = (rand()%(N_TAMANHO_MUNDO - 0 + 1)) + 0;
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

    // criar um conjunto de 2 números aleatórios entre 0 e tamanho do mundo
    int coordx = (rand()%(N_TAMANHO_MUNDO - 0 + 1)) + 0;
    int coordy = (rand()%(N_TAMANHO_MUNDO - 0 + 1)) + 0;
    Missao->Local.x = coordx;
    Missao->Local.y = coordy;
    
    // gerar um conjunto de tam aleatorio entre 6 e 10 com habilidades aleatórias
    int num_habilidades = (rand()%(10 - 6 + 1)) + 6;
    Missao->Habilidades = cjto_aleat(num_habilidades, 10);
    
    return Missao;
}

struct Mundo *cria_mundo()
{
    struct Mundo *Mundo = malloc(sizeof(struct Mundo));
    if(!Mundo)
        return NULL;
    
    Mundo->NMissoes = N_MISSOES;
    Mundo->NHerois = N_HEROIS;
    Mundo->NBases = N_BASES;
    Mundo->NCompostosV = N_COMPOSTOS_V;
    Mundo->Relogio = T_INICIO;
    Mundo->TamanhoMundo = N_TAMANHO_MUNDO;
    Mundo->NHabilidades = N_HABILIDADES;
    Mundo->LEF = fprio_cria();
    
    // Aqui estamos criando um vetor de ponteiros para struct Herois com NHerois espaços de meória
    // Depois disso, estamos completando esses espaços de memória usando o for
    // Essa mesma lógica vai seguir para Missoes e Bases
    Mundo->Herois_vivos = malloc(Mundo->NHerois*(sizeof(struct Heroi *)));
    if(!Mundo->Herois_vivos)
    {
        printf("Erro ao alocar memória para o vetor de heróis");
        free(Mundo); //da free no mundo que havia sido alocado
        return NULL;
    }
    for(int i = 0; i < Mundo->NHerois; i++)
    {
        Mundo->Herois_vivos[i] = cria_heroi(i);
    }

//=====================================================================
    Mundo->Bases = malloc(Mundo->NBases*(sizeof(struct Base *)));
    if(!Mundo->Bases)
    {
        printf("Erro ao alocar memória para o vetor de bases");
        free(Mundo);
        return NULL;
    }
    for(int i = 0; i<Mundo->NBases; i++)
    {
        Mundo->Bases[i] = cria_base(i);
    }
//======================================================================
    Mundo->Missoes = malloc(Mundo->NMissoes*(sizeof(struct Missao *)));
    if(!Mundo->Missoes)
    {
        printf("Erro ao alocar memório para o veto de missoes");
        free(Mundo);
        return NULL;
    }
    for(int i = 0; i < Mundo->NMissoes; i++)
    {
        Mundo->Missoes[i] = cria_missao(i);
    }
    return Mundo;
}




