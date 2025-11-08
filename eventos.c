#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include "eventos.h"
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"

/* para facilitar a compreensão, a orden dessas funções é a mesma ordem 
do Enum dos tipos dos eventos */

void Chega(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    int espera;
    Heroi->Base_Atual = Base->ID;
    
    if(Base->Presentes->num < Base->Lotacao && Base->Espera->num == 0)
        espera = 1; //espera = true
    else
        espera = (Heroi->Paciencia > 10 * Base->Espera->num);
    
    if(espera == 1) 
    {
        struct Evento *evento = malloc(sizeof(struct Evento));
        
        evento->ID_Base = Base->ID;
        evento->ID_heroi = Heroi->ID;
        evento->Tipo = EV_Espera;
        evento->ID_Missao = -1;
        evento->Tempo = Mundo->Relogio;

        fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
        // tem q botar a mensagem de saida ainda
    }
    else 
    {
        struct Evento *evento = malloc(sizeof(struct Evento));
        evento->ID_Base = Base->ID;
        evento->ID_heroi = Heroi->ID;
        evento->Tipo = EV_Desiste;
        evento->ID_Missao = -1;
        evento->Tempo = Mundo->Relogio;

        fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
        // tem q botar a mensagem de saida
    }

}

void Espera(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    fila_insere(Base->Espera, Heroi);
    struct Evento *evento = malloc(sizeof(struct Evento));
    evento->ID_Base = Base->ID;
    evento->ID_heroi = Heroi->ID;
    evento->Tipo = EV_Avisa;
    evento->ID_Missao = -1;
    evento->Tempo = Mundo->Relogio;

    fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
    // tem q botar a mensagem de saida ainda

}

void Desiste(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    int D = rand() % N_BASES;
    Viaja_na_lef(); // passar D como parametro aq 
}

void Avisa(struct Mundo *Mundo, struct Base *Base)
{
    while(Base->Presentes->num < Base->Lotacao && Base->Espera->num > 0){
        struct Heroi *h = fila_retira(Base->Espera);
        cjto_insere(Base->Presentes, h->ID);
        
        // h se refere ao herói que já estava na fila de espera da base
        // "Herói" se refere ao herói que chegou agora

        struct Evento *evento = malloc(sizeof(struct Evento));
        evento->ID_Base = Base->ID;
        evento->ID_heroi = h->ID;
        evento->Tipo = EV_Entra;
        evento->ID_Missao = -1;
        evento->Tempo = Mundo->Relogio;
        fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
        // tem q botar a mensagem de saida ainda
    }
}

void Entra(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    int TPB; //Tempo Permanencia Base
    TPB = 15 + (Heroi->Paciencia * ((rand() % 20) + 1));
    
    struct Evento *evento = malloc(sizeof(struct Evento));
    if(!evento)
    {
        fprintf(stderr, "Erro: falha em Entra\n");
        exit(1);
    }

    evento->ID_heroi = Heroi->ID;
    evento->ID_Base = Base->ID;
    evento->Tipo = EV_Sai;
    evento->Tempo = Mundo->Relogio + TPB;
    evento->ID_Missao = -1;

    fprio_insere(Mundo->LEF,evento,evento->Tipo,evento->Tempo);
    //parâmetros da LEF: a lef, o intem q vai ser inserido, o tipo dele, a prioridade (tempo)
    // tem q botar a mensagem de saida ainda 
}

void Sai(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    int controle;
    controle = cjto_retira(Base->Espera, Heroi);

    if(controle == -1)
        printf("Erro ao retirar o heroi da base");
    
    int indice_nova_base;
    indice_nova_base = rand() % Mundo->NBases; // num aleat entre 0 e num bases.

    struct Evento *viaja = malloc(sizeof(struct Evento));
    viaja->ID_Base = Mundo->Bases[indice_nova_base];
    viaja->ID_heroi = Heroi;
    viaja->ID_Missao = -1;
    viaja->Tempo = Mundo->Relogio;
    viaja->Tipo = EV_Viaja;

    struct Evento *avisa = malloc(sizeof(struct Evento));
    avisa->ID_Base = Mundo->Bases[indice_nova_base];
    avisa->ID_heroi = Heroi;
    avisa->ID_Missao = -1;
    avisa->Tempo = Mundo->Relogio;
    avisa->Tipo = EV_Avisa;

    fprio_insere(Mundo->LEF, viaja, viaja->Tipo, viaja->Tempo);
    fprio_insere(Mundo->LEF, avisa, avisa->Tempo, avisa->Tempo);
    // tem q botar as mensagens de saida aqui paizao
}
void Viaja(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base);
void Morre(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base);
void Missao(struct Mundo *Mundo, struct Missao *Missao);
void Fim(struct Mundo *Mundo);

