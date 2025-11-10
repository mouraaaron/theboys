#ifndef EVENTOS_H
#define EVENTOS_H

#include <stdio.h>

struct Heroi;
struct Base;
struct Missao;
struct Mundo;
struct Loc;
struct Evento;

void select_sort(double matriz[][2], int N);
void Chega(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base);
void Espera(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base);
void Desiste(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base);
void Entra(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base);
void Sai(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base);
void Viaja(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base);
void Morre(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base);
void Missao(struct Mundo *Mundo, struct Missao *Missao);
void Fim(struct Mundo *Mundo);
void Avisa(struct Mundo *Mundo, struct Base *Base);


#endif
