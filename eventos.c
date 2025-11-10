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

/* para facilitar a compreensão, a orden dessas funções é a mesma ordem 
do Enum dos tipos dos eventos */

void select_sort(double matriz[][2], int N) //usado para ordenar a matriz em void Missao
{
    int i,j,min;
    double aux_dist, aux_id;
    for(i = 0; i < N - 1; i++)
    {
        min = i;
        for(j = i + 1; j < N; j++)
        {
            if(matriz[0][j] < matriz[min][0])  
                min = j;
        }
    //realiza as trocas
    aux_dist = matriz[i][0];
    aux_id = matriz[i][1];

    matriz[i][0] = matriz[min][0];
    matriz[i][1] = matriz[min][1];
    
    matriz[min][0] = aux_dist;
    matriz[min][1] = aux_id;

    }
}

void Chega(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    int espera;
    Heroi->Base_Atual = Base;
    
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
    int indice_nova_base = -1;
    do
    {
        indice_nova_base = rand() % Mundo->NBases;
    } while (indice_nova_base == Base->ID);
    

    struct Evento *evento = malloc(sizeof(struct Evento));
    evento->ID_Base = Mundo->Bases[indice_nova_base]->ID;
    evento->ID_heroi = Heroi->ID;
    evento->ID_Missao = -1;
    evento->Tempo = Mundo->Relogio;
    evento->Tipo = EV_Viaja;
    fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
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
    controle = cjto_retira(Base->Presentes, Heroi->ID);

    if(controle == -1)
        printf("Erro ao retirar o heroi da base");
    

    int indice_nova_base = -1;
    do
    {
        indice_nova_base = rand() % Mundo->NBases;
    } while (indice_nova_base == Base->ID);

    
    struct Evento *viaja = malloc(sizeof(struct Evento));
    viaja->ID_Base = Mundo->Bases[indice_nova_base]->ID;
    viaja->ID_heroi = Heroi->ID;
    viaja->ID_Missao = -1;
    viaja->Tempo = Mundo->Relogio;
    viaja->Tipo = EV_Viaja;

    struct Evento *avisa = malloc(sizeof(struct Evento));
    avisa->ID_Base = Mundo->Bases[indice_nova_base]->ID;
    avisa->ID_heroi = Heroi->ID;
    avisa->ID_Missao = -1;
    avisa->Tempo = Mundo->Relogio;
    avisa->Tipo = EV_Avisa;

    fprio_insere(Mundo->LEF, viaja, viaja->Tipo, viaja->Tempo);
    fprio_insere(Mundo->LEF, avisa, avisa->Tempo, avisa->Tempo);
    // tem q botar as mensagens de saida aqui paizao
}

void Viaja(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    struct Base *base_origem = Heroi->Base_Atual;

    int x_origem = base_origem->Local.x;
    int y_origem = base_origem->Local.y;

    int x_destino = Base->Local.x;
    int y_destino = Base->Local.y;

    double D = sqrt((x_destino - x_origem) * (x_destino - x_origem) + (y_destino - y_origem) * (y_destino- y_origem));
    // acima, tem a distancia entre bases

    int duracao_viagem = D / Heroi->Velocidade;

    struct Evento *evento = malloc(sizeof(struct Evento));
    evento->ID_heroi = Heroi->ID;
    evento->ID_Base = Base->ID;
    evento->Tipo = EV_Chega;
    evento->Tempo = Mundo->Relogio + duracao_viagem;
    evento->ID_Missao = -1;

    fprio_insere(Mundo->LEF,evento,evento->Tipo,evento->Tempo);
}

void Morre(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    Heroi->Vida = 0;
    cjto_retira(Base->Presentes, Heroi->ID);
    Avisa(Mundo, Base);
    // botar mensagem de saida 
}

void Missao(struct Mundo *Mundo, struct Missao *Missao)
{
// vou usar matrizes - recebi breve auxílio de um veterano
    double matriz [Mundo->NMissoes][2]; 
    

    for(int i = 0; i < Mundo->NBases; i++)
    {
        int x_base = Mundo->Bases[i]->Local.x;
        int y_base = Mundo->Bases[i]->Local.y;

    
        double D = sqrt((Missao->Local.x - x_base) * (Missao->Local.x - x_base) + (Missao->Local.y - y_base) * (Missao->Local.y - y_base));

        matriz[i][0] = D;
        matriz[i][1] = Mundo->Bases[i]->ID;
    } //completo a matriz com a distancia (da base à missão) e o ID de cada base

    select_sort(matriz, Mundo->NBases);
    
}

void Fim(struct Mundo *Mundo)
{}

    /* menor_d = d_base1;
    for(int i = 1; i < Mundo->NBases; i++) // só < pois o vetor vai de 0..NBases -1
    {
        int x_base = Mundo->Bases[i]->Local.x;
        int y_base = Mundo->Bases[i]->Local.y;

        double distancia = sqrt((Missao->Local.x - x_base) * (Missao->Local.x - x_base) + (Missao->Local.y - y_base) * (Missao->Local.y - y_base));

        if(menor_d > distancia)
            menor_d = distancia; 
    }  */

   
