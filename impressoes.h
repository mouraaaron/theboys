#ifndef IMPRESSOES_H
#define IMPRESSOES_H



void imprime_chega(struct Mundo *Mundo, struct Evento *evento, int espera);
void imprime_espera(struct Mundo *Mundo, struct Evento *evento);
void imprime_desiste(struct Mundo *Mundo, struct Evento *evento);
void imprime_avisa(struct Mundo *Mundo, struct Base *base);
void imprime_avisa_admite(struct Mundo *Mundo, struct Evento *evento, int id_heroi);
void imprime_entra(struct Mundo *Mundo, struct Evento *evento);
void imprime_sai(struct Mundo *Mundo, struct Evento *evento);
void imprime_viaja(struct Mundo *Mundo, struct Evento *evento, int base_origem, int base_destino, double distancia);
void imprime_morre(struct Mundo *Mundo, struct Evento *evento);
void imprime_missao_concluida(struct Mundo *Mundo, struct Evento *evento);
void imprime_missao_impossive(struct Mundo *Mundo, struct Evento *evento);
void imprime_fim(struct Mundo *Mundo, struct Evento *evento);


#endif
