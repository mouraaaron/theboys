#include <stdio.h>
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



void incrementa_xp(struct Mundo *Mundo, int id_base)
{
    if(id_base == -1)
        return;
    // pra nao dar segfault

    for(int id_heroi = 0; id_heroi < Mundo->NHerois; id_heroi ++)
    {
        if(cjto_pertence(Mundo->Bases[id_base]->Presentes, id_heroi))
        {
            if(Mundo->Herois_vivos[id_heroi]!= NULL)
                Mundo->Herois_vivos[id_heroi]->Experiencia++;
        }
    }
}
int heroi_mais_xp(struct Mundo *Mundo, int id_base)
{
    
    int maior_xp = -1;
    int maior_xp_id = -1;
    
    if(Mundo->Bases[id_base]->Presentes == NULL)
        return -1;

    for(int i = 0; i < Mundo->NHerois; i++)
    {
        struct Heroi *heroi = Mundo->Herois_vivos[i];

        if(heroi->Base_Atual == id_base && heroi->Vida > 0)
        {
            if(heroi->Experiencia > maior_xp)
            {
                maior_xp = heroi->Experiencia;
                maior_xp_id = heroi->ID;
            }
        }
    }

    return maior_xp_id;
    // estava tendo um problema: um herói estava morrendo + de uma vez
}
void Chega(struct Mundo *Mundo, struct Evento *evento)
{
    int id_base = evento->ID_Base;
    int id_heroi = evento->ID_heroi;
    int controle_bases_presentes = 0;

    // se o heroi já está no conjunto de presentes de outra base = 1
    for (int i = 0; i < N_BASES; i++) {
        if (cjto_pertence(Mundo->Bases[i]->Presentes, id_heroi)) {
            controle_bases_presentes = 1; 
        }
    }

    if (Mundo->Herois_vivos[id_heroi]->Paciencia == 0 || controle_bases_presentes == 1)
        return;   

    int espera = 0;
    int fila_base = Mundo->Bases[id_base]->Espera->num;

    if(fila_base > Mundo->Bases[id_base]->fila_max)
    {
        Mundo->Bases[evento->ID_Base]->fila_max = fila_base;
        // atualiza a fila maxima que essa base já teve
    }

    if(fila_base == 0 && Mundo->Bases[id_base]->Presentes->num < Mundo->Bases[id_base]->Lotacao)
        espera = 1; //espera = true
    else    
        espera = (Mundo->Herois_vivos[id_heroi]->Paciencia > 10 * fila_base);


    if(espera == 1)
    {
        int hora = Mundo->Relogio;
        int presentes_base = Mundo->Bases[id_base]->Lotacao;
        int capacidade = Mundo->Bases[id_base]->Lotacao;
        espera_na_lef(Mundo, evento);
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA", hora,id_heroi,id_base,presentes_base,capacidade);
        printf("\n");
    }
    else
    {
        int hora = Mundo->Relogio;
        int presentes_base = Mundo->Bases[id_base]->Lotacao;
        int capacidade = Mundo->Bases[id_base]->Lotacao;
        desiste_na_lef(Mundo, evento);
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE",hora,id_heroi,id_base,presentes_base,capacidade);
    }
    

}
void Espera(struct Mundo *Mundo, struct Evento *evento)
{
    int id_heroi = evento->ID_heroi;
    int id_base = evento->ID_Base;
    int hora = Mundo->Relogio;
    int tamanho_fila = fila_tamanho(Mundo->Bases[id_base]->Espera);
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)", hora, id_heroi, id_base, tamanho_fila);
    printf("\n");

    fila_insere(Mundo->Bases[id_base]->Espera, id_heroi);

    int tam_atual = fila_tamanho(Mundo->Bases[id_base]->Espera);

    if(tam_atual > Mundo->Bases[id_base]->fila_max)
        Mundo->Bases[id_base]->fila_max = tam_atual;
   
    avisa_na_lef(Mundo, evento);
}
void Desiste(struct Mundo *Mundo, struct Evento *evento)
{
    int id_heroi = evento->ID_heroi;
    int id_base = evento->ID_Base;
    int hora = Mundo->Relogio;

    if(Mundo->Herois_vivos[id_heroi]->Vida == 0)
        return;
    
    printf("%6d: DESIST HEROI %2d BASE %d\n", hora, id_heroi, id_base);
    int indice_nova_base = -1;
    do
    {
        indice_nova_base = rand() % Mundo->NBases;
    } while (indice_nova_base == Mundo->Bases[id_base]->ID);

    viaja_na_lef(Mundo, evento, indice_nova_base);
}
void Viaja(struct Mundo *Mundo, struct Evento *evento)
{
    int id_heroi = evento->ID_heroi;
    int destino = evento->destino;
    int hora = Mundo->Relogio;

    if(Mundo->Herois_vivos[id_heroi]->Vida == 0)
        return;

    int id_base_origem = evento->ID_Base;


    if(id_base_origem== -1 || destino == -1)
        return;

    int x_origem = Mundo->Bases[id_base_origem]->Local.x;
    int y_origem = Mundo->Bases[id_base_origem]->Local.y;

    int x_destino = Mundo->Bases[destino]->Local.x;
    int y_destino = Mundo->Bases[destino]->Local.y;

    int D = sqrt((x_destino - x_origem) * (x_destino - x_origem) + (y_destino - y_origem) * (y_destino - y_origem));
    // acima, tem a distancia entre bases

    int velo_heroi = Mundo->Herois_vivos[id_heroi]->Velocidade;
    int duracao_viagem = D / velo_heroi;

    int horario_chegada_nova_base = Mundo->Relogio + duracao_viagem;


    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", hora, id_heroi, id_base_origem, evento->ID_Base, D, velo_heroi, horario_chegada_nova_base);
    chega_na_lef(Mundo, evento, duracao_viagem);


}
void Entra(struct Mundo *Mundo, struct Evento *evento)
{
    
    int id_heroi = evento->ID_heroi;
    int id_base = evento->ID_Base;
    int hora = Mundo->Relogio;

    if(Mundo->Herois_vivos[id_heroi]->Vida == 0)
        return;

    Mundo->Herois_vivos[id_heroi]->Base_Atual = evento->ID_Base;
    cjto_insere(Mundo->Bases[id_base]->Presentes, evento->ID_heroi);

    int TPB;
    int heroi_paciencia = Mundo->Herois_vivos[id_heroi]->Paciencia;

    TPB = 15 + (heroi_paciencia * ((rand() % 20) + 1));

    int qtd_presentes = Mundo->Bases[id_base]->Presentes->num;
    int capacidade = Mundo->Bases[id_base]->Lotacao;
    int horario_saida = hora + TPB;
    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", hora, id_heroi, id_base, qtd_presentes, capacidade, horario_saida);

    sai_na_lef(Mundo, evento, TPB);

    
}
void Sai(struct Mundo *Mundo, struct Evento *evento)
{
    /*    if(mundo->Herois[evento->heroi]->vivo ==0){
        mundo->EventosTotais--;
        return;
    }*/

    int id_heroi = evento->ID_heroi;
    int id_base = evento->ID_Base;
    int hora = Mundo->Relogio;

    if(Mundo->Herois_vivos[id_heroi]->Vida == 0)
        return;

    cjto_retira(Mundo->Bases[id_base]->Presentes, id_heroi);

    int qtd_presentes = Mundo->Bases[id_base]->Presentes->num;
    int capacidade = Mundo->Bases[id_base]->Presentes->cap;
    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", hora, id_heroi, id_base, qtd_presentes, capacidade);

    int destino = rand() % N_BASES;
    // nova base aleatória para esse heroi

    Mundo->Herois_vivos[id_heroi]->Base_Atual = -1;
    viaja_na_lef(Mundo, evento, destino);
    avisa_na_lef(Mundo, evento);

}
void Morre(struct Mundo *Mundo, struct Evento *evento)
{
    int id_heroi = evento->ID_heroi;
    int hora = Mundo->Relogio;
    int id_missao = evento->ID_Missao;

    struct Heroi *h = Mundo->Herois_vivos[id_heroi];

    if(h->Vida == 0)
        return; //para nao ter problema com heroi morrendo + de 1 vez
    h->Vida = 0;

    int base = -1;

    if(evento->ID_Base >= 0 && evento->ID_Base < Mundo->NBases)
    {
        base = evento->ID_Base;
    }
    else if(h->Base_Atual >= 0 && h->Base_Atual < Mundo->NBases)
    {
        base = h->Base_Atual;
    }

    if(base != -1)
        cjto_retira(Mundo->Bases[base]->Presentes, id_heroi);

    h->Base_Atual = -1;

    Mundo->total_mortes++;

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", hora, id_heroi, id_missao);
    avisa_na_lef(Mundo, evento);
}
void Avisa(struct Mundo *Mundo, struct Evento *evento)
{
    int id_base = evento->ID_Base;
    int hora = Mundo->Relogio;
    
    if(id_base < 0 || id_base >= Mundo->NBases)
        return;

    struct Base *B = Mundo->Bases[id_base];
    int presentes_base = cjto_card(B->Presentes);
    int capacidade = B->Lotacao;
    
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ", hora, id_base, presentes_base, capacidade);
    printf("[ ");
    fila_imprime(Mundo->Bases[id_base]->Espera);
    printf("]");

    int tam_fila = fila_tamanho(Mundo->Bases[id_base]->Espera);
    int cap = Mundo->Bases[id_base]->Lotacao;

    while(presentes_base < cap && tam_fila > 0)
    {
        int heroi_retirado;
        fila_retira(Mundo->Bases[id_base]->Espera, &heroi_retirado);

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", hora, id_base, heroi_retirado);

        struct Evento evento_entra;
        evento_entra.ID_heroi = heroi_retirado;
        evento_entra.ID_Base = id_base;
        evento_entra.ID_Missao = evento->ID_Missao;
        evento_entra.Tempo = hora;

        entra_na_lef(Mundo, &evento_entra);
        presentes_base++;
        tam_fila--;
        // atualiza as variaveis de controle do while aqui
    }

}
void fim(struct Mundo *Mundo)
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
    float porcentagem = ((float)missoes_cump/(float)Mundo->NMissoes) * 100.0;

    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", missoes_cump, Mundo->NMissoes,porcentagem);

    int min = 31000;
    int max = 0;
    long acumulador = 0;
    for(int w = 0; w < Mundo->NMissoes; w++)
    {

        int t = Mundo->Missoes[w]->tentativas;
        
        if(min > t)
            min = t;

        if(max < t)
            max = t;
    
        acumulador += t;
    }

    float media = (float) acumulador / Mundo->NMissoes;

    
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min, max, media);

    
    float taxa_mortalidade = ((float)Mundo->total_mortes/ (float)Mundo->NHerois) * 100.0;
    printf("TAXA MORTALIDADE: %.1f%%\n", taxa_mortalidade);

}
void eventos_iniciais(struct Mundo *Mundo)
{
  
  for(int i = 0; i < Mundo->NHerois; i++)
  {
    // coloca herois na lef 
    struct Evento *evento = malloc(sizeof(struct Evento));
    evento->Tempo = (rand() % 4320 - 0) + 0;
    evento->ID_Base = (rand() % Mundo->NBases - 0) + 0;
    evento->Tipo = EV_Chega;
    evento->ID_heroi = i;
    evento->ID_Missao = -1;
    

    fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
    
  }

  
  //colocando as missoes nA LEF
  for(int u = 0; u < Mundo->NMissoes; u++)
  {
    struct Evento *ev = malloc(sizeof(struct Evento));
    ev->Tempo = (rand() % T_FIM_DO_MUNDO);
    ev->Tipo = EV_Missao;
    ev->ID_Missao = Mundo->Missoes[u]->ID;
    ev->ID_Base = -1;
    ev->ID_heroi = -1;
    
    fprio_insere(Mundo->LEF, ev, ev->Tipo, ev->Tempo);
  
  }
  
  //settando o fim
  struct Evento *ev_fim = malloc(sizeof(struct Evento));
  ev_fim->Tipo = EV_Fim;
  ev_fim->Tempo = T_FIM_DO_MUNDO;
  ev_fim->ID_Base = -1;
  ev_fim->ID_heroi = -1;
  ev_fim->ID_Missao = -1;

  fprio_insere(Mundo->LEF, ev_fim, ev_fim->Tipo, ev_fim->Tempo);
}
void Missao(struct Mundo *Mundo, struct Evento *evento)
{
    // para cada base, calcular a distancia
    // para cada base, ver se ela tem as habilidades sufiencetes para fazer a missao 
    // dentro das bases que tem as habilidades suficientes, encontrar a com menor distancia
    // se nenhuma tiver as hab suficientes, se pode usar compostoV
    // se puder usar compostoV, queremos a base mais próxima e que tenha heróis dentro

    int hora = Mundo->Relogio;
    int id_missao = evento->ID_Missao;
    Mundo->Missoes[id_missao]->tentativas++;
    int tent = Mundo->Missoes[id_missao]->tentativas;
    int x_missao = Mundo->Missoes[id_missao]->Local.x;
    int y_missao = Mundo->Missoes[id_missao]->Local.y;

    printf("%6d: MISSAO %d TENT %d HAB REQ: [", hora, id_missao, tent);
    cjto_imprime(Mundo->Missoes[id_missao]->Habilidades);
    printf("]\n");
    // esse print aparece independente do caso

    int bmp_id = -1;
    int menor_distancia = 9999999; 

    for (int i = 0; i < Mundo->NBases; i++)
    {
        int id_base = Mundo->Bases[i]->ID;
        int x_base = Mundo->Bases[id_base]->Local.x;
        int y_base = Mundo->Bases[id_base]->Local.y;

        int D = (int)sqrt(((x_missao - x_base) * (x_missao - x_base)) + ((y_missao - y_base) * (y_missao - y_base)));
        // distancia entre a missao e a base


        struct cjto_t *hab_base = cjto_cria(Mundo->NHabilidades);
        // vamos colocar as habilidades da base que estamos analisando nesse conjunto aqui

        for(int k = 0; k < Mundo->NHerois; k++)
        {
            int id_heroi = Mundo->Herois_vivos[k]->ID;
            if(Mundo->Herois_vivos[k] && Mundo->Herois_vivos[id_heroi]->Vida > 0 && Mundo->Herois_vivos[id_heroi]->Base_Atual == id_base)
            {
                // o if verifica se o heroi ta vivo e se ele ta na base que estamos analisando
                struct cjto_t *uniao = cjto_uniao(Mundo->Herois_vivos[id_heroi]->Habilidades, hab_base);
                cjto_destroi(hab_base);
                hab_base = uniao;
            }
            
        }

   
    int capaz = cjto_contem(hab_base, Mundo->Missoes[id_missao]->Habilidades);
    // vai dar 1 ou 0
    if (capaz == 1 && D < menor_distancia)
    {
        menor_distancia = D;
        bmp_id = i;
    }

    cjto_destroi(hab_base);
    } 

    if(bmp_id == -1 && (hora % 2500 == 0) && Mundo->NCompostosV > 0)
    {
  
        int base_perto_id = -1;
        int dist_min = 999999;
        
        for (int k=0; k < Mundo->NBases; k++) {

            int x_base_caso2 = Mundo->Bases[k]->Local.x;
            int y_base_case2 = Mundo->Bases[k]->Local.y;

            int d =  (int)sqrt(((x_missao - x_base_caso2) * (x_missao - x_base_caso2)) + ((y_missao - y_base_case2) * (y_missao - y_base_case2)));
            if (d < dist_min && (Mundo->Bases[k]->Presentes->num != 0)) 
            {
                // ele só dá continuidade se há pelo menos um heroi dentro da base.
                // se continuarmos e nao houver ninguem dentro da base, vamos ter problemas dps
                dist_min = d;
                base_perto_id = k;

            }
        }
        
        if(base_perto_id == -1)
        {
            //quer dizer que NENHUMA base tem pelo menos um cara para ser usado de sacrifício
            printf("%6d: MISSAO %d IMPOSSIVEL\n", hora, id_missao);
            evento->Tempo = hora + (24 * 60); 
            missao_na_lef(Mundo, evento);
            return;
        }
        
        //sacrifica o heroi mais experiente
        int heroi_sacrificio = heroi_mais_xp(Mundo, base_perto_id);

        if(heroi_sacrificio != -1) {   
            Mundo->NCompostosV--;
            Mundo->mi_cumpridas++;

            evento->ID_Base = base_perto_id;
            evento->ID_heroi = heroi_sacrificio;

            Morre(Mundo, evento);
            incrementa_xp(Mundo, base_perto_id);
            Mundo->Bases[base_perto_id]->n_missoes++; 
            
            morre_na_lef(Mundo, evento, heroi_sacrificio, base_perto_id);

            printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [", hora, id_missao, base_perto_id);
            for(int j = 0; j < Mundo->NHerois; j++)
            {
                if(Mundo->Herois_vivos[j]->Vida == 1 && Mundo->Herois_vivos[j]->Base_Atual == base_perto_id)
                cjto_imprime(Mundo->Herois_vivos[j]->Habilidades);
            }
            printf("]\n");
            return;
        }
    }

    if(bmp_id != -1) 
    {
        incrementa_xp(Mundo, bmp_id); 
        Mundo->mi_cumpridas++;
        Mundo->Bases[bmp_id]->n_missoes++; 

        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [", hora, id_missao, bmp_id);
        for(int k = 0; k < Mundo->NHerois; k++) {
            if(Mundo->Herois_vivos[k]->Vida == 1 && Mundo->Herois_vivos[k]->Base_Atual == bmp_id) {
                    cjto_imprime(Mundo->Herois_vivos[k]->Habilidades);
            }
        }
        printf("]\n");
        return;
    }


    printf("%6d: MISSAO %d IMPOSSIVEL\n", hora, id_missao);
    evento->Tempo = hora + (24 * 60); 
    missao_na_lef(Mundo, evento);
}
