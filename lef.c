// definir e cirar a LISTA DE EVENTOS FUTUROS L E F
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




void inicia_lef(struct Mundo *Mundo)
{
    int cond = 1;
    while ( cond == 1 && fprio_tamanho(Mundo->LEF) > 0)
    {
        int prioridade;
        int tipo;
        struct Evento *ev = fprio_retira(Mundo->LEF,&tipo, &prioridade);

        if (ev == NULL)
            break;

        Mundo->Relogio = ev->Tempo;

        /* Constrói ponteiros reais a partir dos IDs */
        struct Heroi *H = NULL;
        struct Base *B = NULL;
        struct Missao *M = NULL;

        if (ev->ID_heroi >= 0)
            H = Mundo->Herois_vivos[ev->ID_heroi];

        if (ev->ID_Base >= 0)
            B = Mundo->Bases[ev->ID_Base];

        if (ev->ID_Missao >= 0)
            M = Mundo->Missoes[ev->ID_Missao];

        /* Processa o evento correto */
        switch (ev->Tipo)
        {
            case EV_Chega:
                Chega(Mundo, H, B);
                break;

            case EV_Espera:
                Espera(Mundo, H, B);
                break;

            case EV_Desiste:
                Desiste(Mundo, H, B);
                break;

            case EV_Avisa:
                Avisa(Mundo, B);
                break;

            case EV_Entra:
                Entra(Mundo, H, B);
                break;

            case EV_Sai:
                Sai(Mundo, H, B);
                break;

            case EV_Viaja:
                Viaja(Mundo, H, B);
                break;

            case EV_Morre:
                Morre(Mundo, H, B, ev->ID_Missao);
                break;

            case EV_Missao:
                Missao(Mundo, M);
                break;

            case EV_Fim:
                cond = 0;
                Fim(Mundo);
                break;
        }

        free(ev);
    }
}

