#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "entidades.h"
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
#include "eventos.h"   // só aqui
//aqui, crio as entidades

struct Heroi *cria_heroi(int ID)
{ 
    struct Heroi *Heroi = malloc(sizeof(struct Heroi));

    if(Heroi == NULL)
        return NULL;
    
    Heroi->ID = ID;
    Heroi-> Paciencia = (rand() % (100 - 0 + 1)) + 0;
    Heroi->Velocidade = (rand() % (5000 - 500 + 1)) + 50;
    Heroi->Experiencia = 0;
    Heroi->Base_Atual->Local.x = 0;
    Heroi->Base_Atual->Local.y = 0; //o que colocar aqui?
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
    Base->fila_max = 0;
    Base->n_missoes = 0;    
    
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
    Missao->tentativas = 0;
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
    Mundo->ev_tratados = 0;
    Mundo->mi_cumpridas= 0;
    Mundo->max_tentativas= 0;
    Mundo->min_tentativs = 0;
    Mundo->total_mortes = 0;

    Mundo->LEF = fprio_cria();
    if(!Mundo->LEF)
    {
        fprintf(stderr, "erro ao criar a lef em cria_mundo");
        free(Mundo);
        return NULL;
    }
    
    // Aqui estamos criando um vetor de ponteiros para struct Herois com NHerois espaços de meória
    // Depois disso, estamos completando esses espaços de memória usando o for
    // Essa mesma lógica vai seguir para Missoes e Bases
    Mundo->Herois_vivos = malloc(Mundo->NHerois*(sizeof(struct Heroi *)));
    if(!Mundo->Herois_vivos)
    {
        fprintf(stderr, "Erro ao alocar memória para o vetor de heróis em cria_mundo");
        free(Mundo->LEF);
        free(Mundo); //da free no mundo que havia sido alocado
        return NULL;
    }

    for(int i = 0; i < Mundo->NHerois; i++)
    {
        Mundo->Herois_vivos[i] = cria_heroi(i);

        if(!Mundo->Herois_vivos[i])
        {
            fprintf(stderr, "erro ao criar um heroi %d e colocar ele no vetor em cria_mundo\n", i);
            
            for(int u = 0; u < i; u++)
            {
                free(Mundo->Herois_vivos[u]);
            }
            
            free(Mundo->Herois_vivos);
            free(Mundo->LEF);
            free(Mundo);
            return NULL;
        }
    }

//=====================================================================
    Mundo->Bases = malloc(Mundo->NBases*(sizeof(struct Base *)));

    if(!Mundo->Bases)
    {
        fprintf(stderr, "Erro ao alocar memória para o vetor de bases");

        for (int j = 0; j < Mundo->NHerois; j++)
            free(Mundo->Herois_vivos[j]);

        free(Mundo->Herois_vivos);
        free(Mundo->LEF);
        free(Mundo);
        return NULL;
    }

    for(int i = 0; i<Mundo->NBases; i++)
    {
        Mundo->Bases[i] = cria_base(i);

        if(!Mundo->Bases[i]) {
            fprintf(stderr, "erro ao colocar uma base no vetor de bases em cria_mundo\n", i);

            for (int j = 0; j < i; j++)
                free(Mundo->Bases[j]);

            free(Mundo->Bases);

            for (int j = 0; j < Mundo->NHerois; j++)
                free(Mundo->Herois_vivos[j]);
            free(Mundo->Herois_vivos);

            free(Mundo->LEF);
            free(Mundo);
            return NULL;
        }
    }
//======================================================================
    Mundo->Missoes = malloc(Mundo->NMissoes*(sizeof(struct Missao *)));
    if(!Mundo->Missoes)
    {
        fprintf(stderr, "Erro ao alocar memório para o veto de missoes");

        for (int j = 0; j < Mundo->NBases; j++)
            free(Mundo->Bases[j]);

        free(Mundo->Bases);
        for (int j = 0; j < Mundo->NHerois; j++)
            free(Mundo->Herois_vivos[j]);

        free(Mundo->Herois_vivos);
        free(Mundo->LEF);
        free(Mundo);
        return NULL;
    }

    for(int i = 0; i < Mundo->NMissoes; i++)
    {
        Mundo->Missoes[i] = cria_missao(i);

        if (!Mundo->Missoes[i]) {
        fprintf(stderr, "erro ao criar uma missao em cria_mundo\n");

            for (int j = 0; j < i; j++)
                free(Mundo->Missoes[j]);
            free(Mundo->Missoes);

        
            for (int j = 0; j < Mundo->NBases; j++)
                free(Mundo->Bases[j]);
            free(Mundo->Bases);

    
            for (int j = 0; j < Mundo->NHerois; j++)
                free(Mundo->Herois_vivos[j]);
            free(Mundo->Herois_vivos);

            free(Mundo->LEF);
            free(Mundo);
            return NULL;
        }
    }
    
    return Mundo;
}
