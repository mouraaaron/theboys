#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include "eventos.h"
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
#include "entidades.h"
#include <math.h>
#include "cria_mundo.h"
#include "lef.h"

void inicia_lef(struct Mundo *Mundo)
{
    int cond = 1;
    while ( cond == 1 && fprio_tamanho(Mundo->LEF) > 0)
    {
        int prioridade;
        int tipo;
        void *item = fprio_retira(Mundo->LEF,&tipo, &prioridade);

        if (item == NULL)
            break;

        switch (tipo)
        {
            case EV_Chega:
                Chega(Mundo, item);
                Mundo->ev_tratados++;
                break;

            case EV_Espera:
                Espera(Mundo, item);
                Mundo->ev_tratados++;
                break;

            case EV_Desiste:
                Desiste(Mundo, item);
                Mundo->ev_tratados++;
                break;

            case EV_Avisa:
                Avisa(Mundo, item);
                Mundo->ev_tratados++;
                break;

            case EV_Entra:
                Entra(Mundo,item);
                Mundo->ev_tratados++;
                break;

            case EV_Sai:
                Sai(Mundo,item);
                Mundo->ev_tratados++;
                break;

            case EV_Viaja:
                Viaja(Mundo,item);
                Mundo->ev_tratados++;
                break;

            case EV_Morre:
                Morre(Mundo,item);
                Mundo->ev_tratados++;
                break;

            case EV_Missao:
                Missao(Mundo, item);
                Mundo->ev_tratados++;
                break;

            case EV_Fim:
                Mundo->ev_tratados++;
                cond = 0;
                fim(Mundo);
                break;
        }

        free(item);
    }
}

void chega_na_lef(struct Mundo *Mundo, struct Evento *evento, int duracao)
{
    struct Evento *Chega  = malloc(sizeof(struct Evento));
    if(Chega == NULL)
        return;
    
    Chega->ID_Base = evento->ID_Base;
    Chega->ID_heroi = evento->ID_heroi;
    Chega->ID_Missao = evento->ID_Missao;
    Chega->Tipo = EV_Chega;
    Chega->Tempo = evento->Tempo + duracao;

    Mundo->Relogio = evento->Tempo;
    fprio_insere(Mundo->LEF, Chega, Chega->Tipo, Chega->Tempo);
}

void espera_na_lef(struct Mundo *Mundo, struct Evento *evento)
{
    struct Evento *Espera = malloc(sizeof(struct Evento));
    if (Espera == NULL)
        return;

    Espera->ID_Base = evento->ID_Base;
    Espera->ID_heroi = evento->ID_heroi;
    Espera->ID_Missao = evento->ID_Missao;
    Espera->Tipo = EV_Espera;
    Espera->Tempo = evento->Tempo;  

    Mundo->Relogio = evento->Tempo;
    fprio_insere(Mundo->LEF, Espera, Espera->Tipo, Espera->Tempo);
}

void desiste_na_lef(struct Mundo *Mundo, struct Evento *evento)
{
    struct Evento *Desiste = malloc(sizeof(struct Evento));
    if(Desiste == NULL)
        return;

    Desiste->ID_Base = evento->ID_Base;
    Desiste->ID_heroi = evento->ID_heroi;   
    Desiste->ID_Missao = evento->ID_Missao;
    Desiste->Tipo = EV_Desiste;
    Desiste->Tempo = evento->Tempo;   

    Mundo->Relogio = evento->Tempo;
    fprio_insere(Mundo->LEF, Desiste, Desiste->Tipo, Desiste->Tempo);
}

void avisa_na_lef(struct Mundo *Mundo, struct Evento *evento)
{
    struct Evento *Avisa = malloc(sizeof(struct Evento));
    if (Avisa == NULL)
        return;

    Avisa->ID_Base = evento->ID_Base;
    Avisa->ID_heroi = evento->ID_heroi;
    Avisa->ID_Missao = evento->ID_Missao;
    Avisa->Tipo = EV_Avisa;
    Avisa->Tempo = evento->Tempo;   

    Mundo->Relogio = evento->Tempo;
    fprio_insere(Mundo->LEF, Avisa, Avisa->Tipo, Avisa->Tempo);

}

void viaja_na_lef(struct Mundo *Mundo, struct Evento *evento, int base_nova)
{
    struct Evento *Viaja = malloc(sizeof(struct Evento));
    if(Viaja == NULL)
        return;
    
    Viaja->Tempo = evento->Tempo;
    Viaja->Tipo = EV_Viaja;
    Viaja->ID_heroi = evento->ID_heroi;
    Viaja->ID_Base = evento->ID_Base;
    Viaja->ID_Missao = evento->ID_Missao;
    Viaja->destino = base_nova;

    fprio_insere(Mundo->LEF, Viaja, Viaja->Tipo, Viaja->Tempo);

}

void entra_na_lef(struct Mundo *Mundo, struct Evento *evento)
{
    struct Evento *Entra = malloc(sizeof(struct Evento));
    if (Entra == NULL)
        return;

    Entra->ID_Base = evento->ID_Base;
    Entra->ID_heroi = evento->ID_heroi;
    Entra->ID_Missao = evento->ID_Missao;
    Entra->Tipo = EV_Entra;
    Entra->Tempo = evento->Tempo;  

    Mundo->Relogio = evento->Tempo;
    fprio_insere(Mundo->LEF, Entra, Entra->Tipo, Entra->Tempo);

}

void sai_na_lef(struct Mundo *Mundo, struct Evento *evento, int TPB)
{
    struct Evento *Sai = malloc(sizeof(struct Evento));
    if(Sai == NULL)
        return;
    
    Sai->Tipo = EV_Sai;
    Sai->Tempo = evento->Tempo + TPB;
    Sai->ID_heroi = evento->ID_heroi;
    Sai->ID_Base = evento->ID_Base;
    Sai->ID_Missao = evento->ID_Missao;
    

    Mundo->Relogio = evento->Tempo;

    fprio_insere(Mundo->LEF, Sai, Sai->Tipo, Sai->Tempo);
}

void morre_na_lef(struct Mundo *Mundo, struct Evento *evento, int id_heroi, int base_que_morre)
{
    struct Evento *Morre = malloc(sizeof(struct Evento));
    if (Morre == NULL) {
        return;
    }

    Morre->ID_Missao = evento->ID_Missao;
    Morre->ID_Base = base_que_morre;
    Morre->ID_heroi = id_heroi;
    Morre->Tipo = EV_Morre;
    Morre->Tempo = evento->Tempo;

    Mundo->Relogio = evento->Tempo;

    fprio_insere(Mundo->LEF, Morre, Morre->Tipo, Morre->Tempo);
}

void missao_na_lef(struct Mundo *Mundo, struct Evento *evento)
{
    struct Evento *Missao = malloc(sizeof(struct Evento));
    if (Missao == NULL)
        return;

    Missao->ID_Base = evento->ID_Base;
    Missao->ID_heroi = evento->ID_heroi;
    Missao->ID_Missao = evento->ID_Missao;
    Missao->Tipo = EV_Missao;
    Missao->Tempo = evento->Tempo;   

    Mundo->Relogio = evento->Tempo;
    fprio_insere(Mundo->LEF, Missao, Missao->Tipo, Missao->Tempo);

}
