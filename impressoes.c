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
void imprime_desiste(struct Mundo *Mundo, struct Evento *evento)
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
void imprime_avisa_admite(struct Mundo *Mundo, struct Evento *evento, int id_heroi)
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
    
    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", horario, heroi, base_origem, base_destino, distancia, velo, horario_chegada_nova_base);
}
void imprime_morre(struct Mundo *Mundo, struct Evento *evento)
{
    int heroi = evento->ID_heroi;
    int horario = evento->Tempo;
    int id_missao = evento->ID_Missao;
    printf("%6d: MORRE HEROI %2d MISSAO %d\n", horario, heroi, id_missao);
}
void imprime_missao_concluida(struct Mundo *Mundo, struct Evento *evento);
void imprime_missao_impossive(struct Mundo *Mundo, struct Evento *evento);
void imprime_fim(struct Mundo *Mundo, struct Evento *evento);

    
