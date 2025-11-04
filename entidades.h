#ifndef entidades_h
#define entidades_h

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)
#define N_COMPOSTOS_V (N_HABILIDADES * 3)

// PERGUNTA 01: Quanto coloco conjuntos, tem q usar ponteiro?????????

// Aqui, tem todas os protótipos das structs das entidades e das funções de criação das entidades.
struct Heroi;
struct Base;
struct Missao;
struct Mundo;

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
    int Base_Atual;
    int Vida; //1 ou 0 (se for 1 tá vivo, se for 0 tá morto)

};

struct Base{
    int ID;
    int Lotacao;
    struct cjto_t *Presentes; //tem q usar conjuntos 
    struct fprio_t *Espera; //fila de espera
    struct cjto_t *Local; //tem q usar conjunto talvez
};

struct Missao{
    int ID;
    struct cjto_t *Habilidades; //tem q usar conjuntos eu acho 
    struct cjto_t *Local; //tem q usar conjnunto talvez 
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

    /* ADENDO: Por que tem que ter dois **? Porque uma struct Heroi **Herois_vivos é um 
    * ponteiro para um vetor de ponteiros, no qual cada espaço é ocupado por uma struct Heroi *
    * O mesmo vale para missoes e bases.
    */
};


#endif
