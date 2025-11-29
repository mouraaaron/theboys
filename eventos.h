#ifndef EVENTOS_H
#define EVENTOS_H

#include <stdio.h>

struct Heroi;
struct Base;
struct Missao;
struct Mundo;
struct Loc;
struct Evento;


void Chega(struct Mundo *Mundo, struct Evento *evento);
void Espera(struct Mundo *Mundo, struct Evento *evento);
void Desiste(struct Mundo *Mundo, struct Evento *evento);
void Viaja(struct Mundo *Mundo, struct Evento *evento);
void Entra(struct Mundo *Mundo, struct Evento *evento);
void Sai(struct Mundo *Mundo, struct Evento *evento);
void Morre(struct Mundo *Mundo, struct Evento *evento);
void Avisa(struct Mundo *Mundo, struct Evento *evento);
void Missao(struct Mundo *Mundo, struct Evento *evento);
void fim(struct Mundo *Mundo);
void eventos_iniciais(struct Mundo *Mundo);
void destroi_mundo(struct Mundo *Mundo);

#endif
