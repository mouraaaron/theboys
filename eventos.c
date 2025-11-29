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



void incrementa_xp(struct Mundo *Mundo, int id_base)
{
    if(id_base < 0 || id_base>= Mundo->NBases) 
        return;

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
    if(!Mundo || id_base < 0 || id_base >= Mundo->NBases)  
        return -1;
    
    int maior_xp = -1;
    int maior_xp_id = -1;
    
    for(int id_heroi = 0; id_heroi < Mundo->NHerois; id_heroi++)
    {  
        int permissao_para_usar = 1;

    // percorre todos os herois do mundo
        if(Mundo->Herois_vivos[id_heroi] == NULL)
            permissao_para_usar = 0;

        if(cjto_pertence(Mundo->Bases[id_base]->Presentes, id_heroi) == 1)
            permissao_para_usar = 0;

        if(permissao_para_usar == 1)
        {
        // se o herio pertencer a essa base e ele nao morreu, continua
            if(Mundo->Herois_vivos[id_heroi]->Experiencia > maior_xp)
            {
                maior_xp = Mundo->Herois_vivos[id_heroi]->Experiencia;
                maior_xp_id = id_heroi;
            }
           
        }
    }
    return maior_xp_id;
}
void select_sort(double matriz[][3], int N) // usado para ordenar a matriz em void Missao
{
    int i, j, min;
    double aux_dist, aux_id;
    for (i = 0; i < N - 1; i++)
    {
        min = i;
        for (j = i + 1; j < N; j++)
        {
            if (matriz[j][0] < matriz[min][0])
                min = j;
        }
        // realiza as trocas
        aux_dist = matriz[i][0];
        aux_id = matriz[i][1];

        matriz[i][0] = matriz[min][0];
        matriz[i][1] = matriz[min][1];

        matriz[min][0] = aux_dist;
        matriz[min][1] = aux_id;
    }
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
    

    if(Mundo->Herois_vivos[id_heroi]->Vida == 0)
        return; //para debug
    
    fila_insere(Mundo->Bases[id_base]->Espera, id_heroi);

    int tamanho_fila = fila_tamanho(Mundo->Bases[id_base]->Espera);
    if(tamanho_fila > Mundo->Bases[id_base]->fila_max)
        Mundo->Bases[id_base]->fila_max = tamanho_fila;
   
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)", hora, id_heroi, id_base, tamanho_fila);
    printf("\n");
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
void Missao(struct Mundo *Mundo, struct Evento *evento)
{
    
    int hora = Mundo->Relogio;
    int id_missao = evento->ID_Missao;

    // vou usar matrizes - recebi breve auxílio de um veterano
    double matriz[Mundo->NBases][3];

    for (int i = 0; i < Mundo->NBases; i++)
    {
        int x_base = Mundo->Bases[i]->Local.x;
        int y_base = Mundo->Bases[i]->Local.y;

        struct cjto_t *hab_base = cjto_cria(Mundo->NHabilidades);
        // crio um conjunto que vai ter TODAS as habilidaes presentes na base
        for (int id_heroi = 0; id_heroi < Mundo->NHerois; id_heroi++)
        {
            if(cjto_pertence(Mundo->Bases[i]->Presentes, id_heroi))
            {
                // se o id do herói está na base
                struct cjto_t *uniao = cjto_uniao(Mundo->Herois_vivos[id_heroi]->Habilidades, hab_base);
                // une as habilidades do herói com o conjunto de habilidades da base 
                cjto_destroi(hab_base);
                hab_base = uniao;
            }
            
        }
        
        int D = sqrt((Mundo->Missoes[evento->ID_Missao]->Local.x- x_base) * (Mundo->Missoes[evento->ID_Missao]->Local.x- x_base) + (Mundo->Missoes[evento->ID_Missao]->Local.y- y_base) * (Mundo->Missoes[evento->ID_Missao]->Local.y- y_base));

        matriz[i][0] = D;
        matriz[i][1] = Mundo->Bases[i]->ID;
        matriz[i][2] = cjto_contem(hab_base, Mundo->Missoes[evento->ID_Missao]->Habilidades);
        cjto_destroi(hab_base);
    } 

/*acima, estou completando uma matriz de 10 inhas e 3 colunas. Cada linha representa cada base
* a primeira coluna tem a distancia entre a base e a missao 
* a segunda coluna tem o id da base
* a terceira coluna tem 1 ou 0, 1 sendo se a base tem as habilidades suficientes 
* para completar a missao e 0 se nao tem
 */ 

    Mundo->Missoes[evento->ID_Missao]->tentativas++; //aumenta a qtd de tentaticas para imprimir depois

    select_sort(matriz, Mundo->NBases);
    // ordena as linhas pela 1 coluna (distancia entre base e missao)


    int bmp = -1;
    for(int y = 0; y < Mundo->NBases; y++)
    {
        if(matriz[y][2] == 1 ){
            
            bmp = matriz[y][1];
            break;
        }
    // a primeira base tem todas as habilidades necessárias para completar a missao, é a bmp
    }

    if(bmp == -1 && (Mundo->Relogio % 2500 == 0) && Mundo->NCompostosV > 0)
    {
    // se nao há bmp mas podemos usar compostos V
        Mundo->NCompostosV--;
        Mundo->mi_cumpridas++;
        
        int base_heroi_morre = (int) matriz[0][1];

        int heroi_que_vai_morrer = heroi_mais_xp(Mundo, base_heroi_morre);

        evento->ID_heroi = heroi_que_vai_morrer;
        if(heroi_que_vai_morrer == -1)
        {
            //nao tem nenhum heroi pra morrer nessa base, logo essa base deve estar vazia
            //entao a missao é impossível
            printf("%6d: MISSAO %d IMPOSSIVEL ", hora, id_missao);
            printf("\n");
            evento->Tempo = evento->Tempo + (24 * 60); //agendar para o outro dia
            missao_na_lef(Mundo, evento);
            return;
        }

        Morre(Mundo, evento);
        //  em morre já setta a vida dele pra 0 e tira ele do conjunto de presentes da base

        incrementa_xp(Mundo, base_heroi_morre);
        
        Mundo->Bases[base_heroi_morre]->n_missoes++;
        morre_na_lef(Mundo, evento, heroi_que_vai_morrer, base_heroi_morre);

        int tentativas = Mundo->Missoes[id_missao]->tentativas;

        printf("%6d: MISSAO %d TENT %d HAB REQ: ", hora, id_missao, tentativas);
        printf("[");
        cjto_imprime(Mundo->Missoes[id_missao]->Habilidades);
        printf("]");
        printf("\n");

        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: ", hora, id_missao, base_heroi_morre);
        printf("[");
        for(int i = 0; i < Mundo->NHerois; i++)
        {
       
            if(Mundo->Herois_vivos[i] == NULL)
            {
                continue;
            }

            if(Mundo->Herois_vivos[i]->Vida == 1)
            {
                if(cjto_pertence(Mundo->Bases[base_heroi_morre]->Presentes, Mundo->Herois_vivos[i]->ID == 1))
                {
                    cjto_imprime(Mundo->Herois_vivos[i]->Habilidades);
                }
            }
        }
        printf("]");
        printf("\n");
    }
    if(bmp != -1)
    {
        incrementa_xp(Mundo, bmp);
        Mundo->mi_cumpridas++;
        Mundo->Bases[bmp]->n_missoes++;
        int tentativas = Mundo->Missoes[id_missao]->tentativas;

        printf("%6d: MISSAO %d TENT %d HAB REQ: ", hora, id_missao, tentativas);
        printf("[");
        cjto_imprime(Mundo->Missoes[id_missao]->Habilidades);
        printf("]");
        printf("\n");

        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: ", hora, id_missao, bmp);
        printf("[");
        for(int i = 0; i < Mundo->NHerois; i++)
        {
       
            if(Mundo->Herois_vivos[i] == NULL)
            {
                continue;
            }

            if(Mundo->Herois_vivos[i]->Vida == 1)
            {
                if(cjto_pertence(Mundo->Bases[bmp]->Presentes, Mundo->Herois_vivos[i]->ID))
                {
                    cjto_imprime(Mundo->Herois_vivos[i]->Habilidades);
                }
            }
        }
        printf("]");
        printf("\n");

    }
    if(bmp == -1 &&  (Mundo->Relogio % 2500 == 1))
    {
        //missao impossivel
        printf("%6d: MISSAO %d IMPOSSIVEL ", hora, id_missao);
        printf("\n");
        evento->Tempo = evento->Tempo + (24 * 60); //agendar para o outro dia
        missao_na_lef(Mundo, evento);
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
    float porcentagem = missoes_cump/Mundo->NMissoes;
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", missoes_cump, Mundo->NMissoes,porcentagem);

    int min = 31000;
    int max = 0;
    long acumulador = 0;
    for(int w = 0; w < Mundo->NMissoes; w++)
    {
        int id_missao = Mundo->Missoes[w]->ID;
        if(min > Mundo->Missoes[id_missao]->tentativas)
            min = Mundo->Missoes[id_missao]->tentativas;

        if(max < Mundo->Missoes[id_missao]->tentativas)
            max = Mundo->Missoes[id_missao]->tentativas;
    
        acumulador += Mundo->Missoes[id_missao]->tentativas;
    }
    float media = 0.0;
    media = (float)acumulador / Mundo->NMissoes;
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min, max, media);

    float taxa_mortalidade = Mundo->total_mortes/Mundo->NHerois;
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
