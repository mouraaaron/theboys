// definir e cirar a LISTA DE EVENTOS FUTUROS L E F
#ifndef LEF_H
#define LEF_H
#include <stdio.h>
#include "entidades.h"


// aqui, inicio a lef
void inicia_lef(struct Mundo *Mundo);

// aqui, eu insiro esses eventos na lef para melhorar modularizacao no arquivo eventos.c
void chega_na_lef(struct Mundo *Mundo, struct Evento *evento, int duracao);
void espera_na_lef(struct Mundo *Mundo, struct Evento *evento);
void desiste_na_lef(struct Mundo *Mundo, struct Evento *evento);
void avisa_na_lef(struct Mundo *Mundo, struct Evento *evento);
void viaja_na_lef(struct Mundo *Mundo, struct Evento *evento, int id_base_nova);
void entra_na_lef(struct Mundo *Mundo, struct Evento *evento);
void sai_na_lef(struct Mundo *Mundo, struct Evento *evento, int TPB);
void morre_na_lef(struct Mundo *Mundo, struct Evento *evento, int id_heroi, int base_que_morre);
void missao_na_lef(struct Mundo *Mundo, struct Evento *evento);


#endif


