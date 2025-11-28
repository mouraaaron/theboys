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
#include "impressoes.h"


void imprime_chega(struct Mundo *Mundo, struct Evento *evento, int espera)
{
    // tem o int espera pois imprime ESPERA ou DESISTE dependendo dessa int 
    int Heroi = evento->ID_heroi;
    int horario = evento->Tempo;
    int id_base = evento->ID_Base;
    int qtd_presentes = Mundo->Bases[id_base]->Presentes->num;
    int capacidade = Mundo->Bases[id_base]->Lotacao;
    
    if(espera == 1)
    {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n",horario, Heroi, id_base, qtd_presentes, capacidade);
    }
    else
    {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE", horario, Heroi, id_base, qtd_presentes, capacidade);
    }
}
void imprime_espera(struct Mundo *Mundo, struct Evento *evento)
{
    int heroi = evento->ID_heroi;
    int horario = evento->Tempo;
    int id_base = evento->ID_Base;
    int pessoas_na_espera = Mundo->Bases[id_base]->Espera->num;

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", horario, heroi, id_base, pessoas_na_espera);
}
void imprime_desiste(struct Evento *evento)
{
    int heroi = evento->ID_heroi;
    int horario = evento->Tempo;
    int id_base = evento->ID_Base;

    printf("%6d: DESIST HEROI %2d BASE %d\n", horario, heroi, id_base);
}
void imprime_avisa(struct Mundo *Mundo, struct Base *Base)
{
    int horario = Mundo->Relogio;
    int id_base = Base->ID;
    int qtd_presentes = Mundo->Bases[id_base]->Presentes->num;
    int capacidade = Mundo->Bases[id_base]->Lotacao;

    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d)",horario, id_base, qtd_presentes,capacidade);
    printf("FILA [");
    fila_imprime(Mundo->Bases[id_base]->Espera);
    printf("]\n");
    
}
void imprime_avisa_admite(struct Evento *evento, int id_heroi)
{
    int horario = evento->Tempo;
    int id_base = evento->ID_Base;
    printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", horario, id_base, id_heroi);
}
void imprime_entra(struct Mundo *Mundo, struct Evento *evento)
{
    int heroi = evento->ID_heroi;
    int horario = Mundo->Relogio;
    int horario_saida = evento->Tempo;
    int id_base = evento->ID_Base;
    int qtd_presentes = Mundo->Bases[id_base]->Presentes->num;
    int capacidade = Mundo->Bases[id_base]->Lotacao;
    
    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", horario, heroi, id_base, qtd_presentes, capacidade, horario_saida);
}
void imprime_sai(struct Mundo *Mundo, struct Evento *evento)
{
    int heroi = evento->ID_heroi;
    int horario = evento->Tempo;
    int id_base = evento->ID_Base;
    int qtd_presentes = Mundo->Bases[id_base]->Presentes->num;
    int capacidade = Mundo->Bases[id_base]->Lotacao;

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", horario, heroi, id_base, qtd_presentes, capacidade);
}
void imprime_viaja(struct Mundo *Mundo, struct Evento *evento, int base_origem, int base_destino, double distancia)
{
    int heroi = evento->ID_heroi;
    int horario = Mundo->Relogio;
    int horario_chegada_nova_base = evento->Tempo;
    int velo = Mundo->Herois_vivos[heroi]->Velocidade;
    int d_int = (int) distancia;
    
    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", horario, heroi, base_origem, base_destino, d_int, velo, horario_chegada_nova_base);
}
void imprime_morre(struct Evento *evento)
{
    int heroi = evento->ID_heroi;
    int horario = evento->Tempo;
    int id_missao = evento->ID_Missao;
    printf("%6d: MORRE HEROI %2d MISSAO %d\n", horario, heroi, id_missao);
}
void imprime_missao_concluida(struct Mundo *Mundo, struct Evento *evento, int id_bmp)
{
    int tempo = Mundo->Relogio;
    int id = evento->ID_Missao;
    int tentativas = Mundo->Missoes[evento->ID_Missao]->tentativas;
    

    printf("%6d: MISSAO %d TENT %d HAB REQ: ", tempo, id, tentativas);
    printf("[");
    cjto_imprime(Mundo->Missoes[id]->Habilidades);
    printf("]");
    printf("\n");

    printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: ", tempo, id, id_bmp);
    printf("[");
    for(int i = 0; i < Mundo->NHerois; i++)
    {
       
        if(Mundo->Herois_vivos[i] == NULL)
        {
            continue;
        }

        if(Mundo->Herois_vivos[i]->Vida == 1)
        {
            if(cjto_pertence(Mundo->Bases[id_bmp]->Presentes, Mundo->Herois_vivos[i]->ID == 1))
            {
                cjto_imprime(Mundo->Herois_vivos[i]->Habilidades);
            }
        }
    }
    printf("]");
    printf("\n");
}
void imprime_missao_impossivel(struct Mundo *Mundo, struct Evento *evento)
{
    int tempo = Mundo->Relogio;
    int id = evento->ID_Missao;
    
    printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, id);
}
void imprime_fim(struct Mundo *Mundo)
{
    int tempo = Mundo->Relogio;
    printf("%d: FIM\n", tempo);

//============================================
//  impressao dos herois
    for(int i = 0; i <Mundo->NHerois; i++){
        int id_heroi = Mundo->Herois_vivos[i]->ID;
        int pac_heroi = Mundo->Herois_vivos[i]->Paciencia;
        int velo_heroi = Mundo->Herois_vivos[i]->Velocidade;
        int xp_heroi = Mundo->Herois_vivos[i]->Experiencia;

        if(Mundo->Herois_vivos[i]->Vida == 1)
        {
            
            printf("HEROI %2d VIVO  PAC %3d VEL %4d EXP %4d  HABS [", id_heroi, pac_heroi, velo_heroi, xp_heroi);
            cjto_imprime(Mundo->Herois_vivos[id_heroi]->Habilidades);
            printf("]\n");
        }
        else{
            printf("HEROI %2d MORTO  PAC %3d VEL %4d EXP %4d  HABS [", id_heroi, pac_heroi, velo_heroi, xp_heroi);
            cjto_imprime(Mundo->Herois_vivos[id_heroi]->Habilidades);
            printf("]\n");
        }
    }
//==============================================
// impressao das bases
    for(int u = 0; u < Mundo->NBases; u++)
    {
        int id_base = Mundo->Bases[u]->ID;
        int lot_base = Mundo->Bases[id_base]->Lotacao;
        int maior_fila = Mundo->Bases[id_base]->fila_max;
        int numero_missoes = Mundo->Bases[id_base]->n_missoes;

        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", id_base, lot_base, maior_fila,numero_missoes);
    }

//==============================================
// demais impressões
    int eve_tratados = Mundo->ev_tratados;
    printf("EVENTOS TRADADOS: %d\n",eve_tratados);

    int missoes_cump = Mundo->mi_cumpridas;
    float porcentagem = missoes_cump/Mundo->NMissoes;
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", missoes_cump, Mundo->NMissoes,porcentagem);

    int min = 31000;
    int max = 0;
    int acumulador = 0;
    for(int w = 0; w < Mundo->NBases; w++)
    {
        int id_missao = Mundo->Missoes[w]->ID;
        if(min > Mundo->Missoes[id_missao]->tentativas)
            min = Mundo->Missoes[id_missao]->tentativas;

        if(max < Mundo->Missoes[id_missao]->tentativas)
            max = Mundo->Missoes[id_missao]->tentativas;
    
        acumulador += Mundo->Missoes[id_missao]->tentativas;
    }
    float media = acumulador/Mundo->NMissoes;
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min, max, media);

    float taxa_mortalidade = Mundo->total_mortes/Mundo->NHerois;
    printf("TAXA MORTALIDADE: %.1f%%\n", taxa_mortalidade);
}


