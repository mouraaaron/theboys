#ifndef entidades_h
#define entidades_h
#include "eventos.h"
#include <stdio.h>

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)
#define N_COMPOSTOS_V (N_HABILIDADES * 3)

// Aqui, tem todas os protótipos das structs das entidades e das funções de criação das entidades.
struct Heroi;
struct Base;
struct Missao;
struct Mundo;
struct Loc;
struct Evento;

struct Loc
{
    int x;
    int y;
};

//pergunta -> o que passar nos parametros dessas funções 
struct Heroi *cria_heroi(int ID);
struct Base *cria_base(int ID);
struct Missao *cria_missao(int ID);
struct Mundo *cria_mundo();

struct Heroi{
    int ID;
    struct cjto_t *Habilidades; //tem que usar o conjunto eu acho 
    int Paciencia;
    int Velocidade;
    int Experiencia;
    struct Base *Base_Atual;
    int Vida; //1 ou 0 (se for 1 tá vivo, se for 0 tá morto)

};

struct Base{
    int ID;
    int Lotacao;
    struct cjto_t *Presentes; //tem q usar conjuntos 
    struct fila_t *Espera; //fila de espera
    struct Loc Local;
    int fila_max;
    int n_missoes;
};

struct Missao{
    int ID;
    struct cjto_t *Habilidades; //tem q usar conjuntos eu acho 
    struct Loc Local;
    int tentativas;
    
};

struct Mundo{
    int NHerois;
    struct Heroi **Herois_vivos;
    int NBases;
    struct Base **Bases;
    int NMissoes;
    struct Missao **Missoes;
    int NHabilidades;
    int NCompostosV;
    int TamanhoMundo; //tem q usar conjunto eu acho 
    long Relogio;   
    struct fprio_t *LEF;
    int total_mortes;
    int ev_tratados;
    int mi_cumpridas;
    int max_tentativas;
    int min_tentativs;

    /* ADENDO: Por que tem que ter dois **? Porque uma struct Heroi **Herois_vivos é um 
    * ponteiro para um vetor de ponteiros, no qual cada espaço é ocupado por uma struct Heroi *
    * O mesmo vale para missoes e bases.
    */
};

enum TipoEvento
{
    EV_Chega,
    EV_Espera,
    EV_Desiste,
    EV_Avisa, 
    EV_Entra,
    EV_Sai,
    EV_Viaja,
    EV_Morre,
    EV_Missao,
    EV_Fim
};

struct Evento
{
    int ID_heroi;
    int ID_Missao;
    long Tempo;
    enum TipoEvento Tipo;
    int ID_Base;

};

#endif
