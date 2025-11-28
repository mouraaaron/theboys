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

void fila_esvazia(struct fila_t *f)
{
    if (!f) return;

    struct fila_nodo_t *aux = f->prim;

    while (aux != NULL) {
        struct fila_nodo_t *temp = aux->prox;
        free(aux);
        aux = temp;
    }

    f->prim = NULL;
    f->fim = NULL;

}

int fila_contem(struct fila_t *f, void *item) {
    if (!f)
        return 0; // fila vazia ou inválida, não contém

    struct fila_nodo_t *aux = f->prim;
    while (aux) {
        if (aux->item == item)  // compara ponteiros
            return 1;           // já está na fila
        aux = aux->prox;
    }
    return 0; // não encontrou
}

void incrementa_xp(struct Mundo *Mundo, int id_base)
{
    for(int id_heroi = 0; id_heroi < Mundo->NHerois; id_heroi ++)
    {
        if(cjto_pertence(Mundo->Bases[id_base]->Presentes, id_heroi))
        {
            if(Mundo->Herois_vivos[id_heroi]!= NULL)
                Mundo->Herois_vivos[id_heroi]->Experiencia++;
        }
    }
}

void remove_heroi(struct Mundo *Mundo, int id_base, int id_heroi)
{
    cjto_retira(Mundo->Bases[id_base]->Presentes, id_heroi);
    //settar tudo dele null e tirar da base
    Mundo->Herois_vivos[id_heroi]->Vida = 0;
    Mundo->Herois_vivos[id_heroi] = NULL;
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

void Chega(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    int controle_bases_presentes = 0;
    // se o heroi já está no conjunto de presentes de outra base = 1

    for (int i = 0; i < N_BASES; i++) {
        if (cjto_pertence(Mundo->Bases[i]->Presentes, Heroi->ID)) {
            controle_bases_presentes = 1; 
        }
    }

    if (Heroi->Vida == 0 || controle_bases_presentes == 1)
        return;   

    int espera;
    Heroi->Base_Atual = Base;

    if (Base->Presentes->num < Base->Lotacao && Base->Espera->num == 0)
        espera = 1; // espera = true
    else
        espera = (Heroi->Paciencia > 10 * Base->Espera->num);


    // ESSE É UM EVENTO LOCAL USADO PARA IMPRESSÃO
    // faço isso em todos os eventos desse arquivo
    struct Evento ev_chega;
    ev_chega.ID_Base = Base->ID;
    ev_chega.Tempo = Mundo->Relogio;
    ev_chega.ID_heroi = Heroi->ID;
    ev_chega.Tipo = EV_Chega;
    ev_chega.ID_Missao = -1;
    
    imprime_chega(Mundo, &ev_chega, espera);
    if (espera == 1)
    {
        
        struct Evento *evento = malloc(sizeof(struct Evento));

        evento->ID_Base = Base->ID;
        evento->ID_heroi = Heroi->ID;
        evento->Tipo = EV_Espera;
        evento->ID_Missao = -1;
        evento->Tempo = Mundo->Relogio;

        fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
        
        Espera(Mundo, Heroi, Base);
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
    Mundo->ev_tratados++;
}

void Espera(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    int controle_bases_presentes = 0;
    // se o heroi já está no conjunto de presentes de outra base = 1

    for (int i = 0; i < N_BASES; i++) {
        if (cjto_pertence(Mundo->Bases[i]->Presentes, Heroi->ID)) {
            controle_bases_presentes = 1; 
        }
    }

    if (Heroi->Vida == 0 || controle_bases_presentes == 1)
        return;   

    // ESSE É UM EVENTO LOCAL USADO PARA IMPRESSÃO
    struct Evento ev_espera;
    ev_espera.ID_heroi = Heroi->ID;
    ev_espera.Tempo = Mundo->Relogio;
    ev_espera.ID_Base = Base->ID;
    ev_espera.ID_Missao = -1;
    ev_espera.Tipo = EV_Espera;

    imprime_espera(Mundo, &ev_espera); 

    if (!fila_contem(Base->Espera, Heroi)) 
    {
        fila_insere(Base->Espera, Heroi);
    }
    else
    {
        printf("Herói %d já está na fila de espera da base %d\n", Heroi->ID, Base->ID);
    }

    
    struct Evento *evento = malloc(sizeof(struct Evento));
    evento->ID_Base = Base->ID;
    evento->ID_heroi = Heroi->ID;
    evento->Tipo = EV_Avisa;
    evento->ID_Missao = -1;
    evento->Tempo = Mundo->Relogio;
    Mundo->ev_tratados++;

    
    if(Base->Espera->num > Base->fila_max)
        Base->fila_max = Base->Espera->num;

    
    fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
    
}

void Desiste(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    if (Heroi->Vida == 0)
        return;
    struct Evento ev_desiste;
    ev_desiste.ID_Base = Base->ID;
    ev_desiste.Tempo = Mundo->Relogio;
    ev_desiste.ID_heroi = Heroi->ID;
    ev_desiste.ID_Missao = -1;
    ev_desiste.Tipo = EV_Desiste; 
    imprime_desiste(&ev_desiste);

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
    Mundo->ev_tratados++;

    fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
}

void Avisa(struct Mundo *Mundo, struct Base *Base)
{
  
    imprime_avisa(Mundo, Base);

    if(Base->Espera->num < 0){
        printf("debugando , fila tem tamanho <0 vamo esvaziar tudo para resetar");
        fila_esvazia(Base->Espera);
    }
    while (Base->Presentes->num < Base->Lotacao && Base->Espera->num > 0)
    {
        struct Heroi *h = fila_retira(Base->Espera);
        if(h == NULL)
            break;
        
    
        if(cjto_pertence(Base->Presentes, h->ID) == 1)
        {
            printf("Debug AVISA -> fila já tem o heroi que tentamos adicionar");
            break;
        }

        cjto_insere(Base->Presentes, h->ID);
    

        // h se refere ao herói que já estava na fila de espera da base
        // "Herói" se refere ao herói que chegou agora

        struct Evento evento_local;
        // evento local para impressao do heroi que entrou na base
        evento_local.ID_Base = Base->ID;
        evento_local.ID_heroi = h->ID;
        evento_local.ID_Missao = -1;
        evento_local.Tempo = Mundo->Relogio;
        evento_local.Tipo = EV_Entra;
        imprime_avisa_admite(&evento_local, h->ID);


        struct Evento *evento = malloc(sizeof(struct Evento));
        if(!evento)
        {
            fprintf(stderr, "Erro: malloc falhou debugando AVISA\n");
            exit(1);
        }

        evento->ID_Base = Base->ID;
        evento->ID_heroi = h->ID;
        evento->Tipo = EV_Entra;
        evento->ID_Missao = -1;
        evento->Tempo = Mundo->Relogio;
        fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
        // tem q botar a mensagem de saida ainda

        
    }
    Mundo->ev_tratados++;
}

void Entra(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    if (Heroi->Vida == 0)
        return;
    int TPB; // Tempo Permanencia Base
    TPB = 15 + (Heroi->Paciencia * ((rand() % 20) + 1));

    struct Evento impressao;
    impressao.ID_Base = Base->ID;
    impressao.ID_heroi = Heroi->ID;
    impressao.Tempo = Mundo->Relogio;
    impressao.Tipo = EV_Entra;
    impressao.ID_Missao = -1;
    imprime_entra(Mundo, &impressao);

    struct Evento *evento = malloc(sizeof(struct Evento));
    if (!evento)
    {
        fprintf(stderr, "Erro: falha em Entra\n");
        exit(1);
    }

    evento->ID_heroi = Heroi->ID;
    evento->ID_Base = Base->ID;
    evento->Tipo = EV_Sai;
    evento->Tempo = Mundo->Relogio + TPB;
    evento->ID_Missao = -1;

    fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
    Mundo->ev_tratados++;
    // parâmetros da LEF: a lef, o intem q vai ser inserido, o tipo dele, a prioridade (tempo)
    //  tem q botar a mensagem de saida ainda
}

void Sai(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{
    int controle;
    controle = cjto_retira(Base->Presentes, Heroi->ID);

    if (controle == -1)
        printf("Erro ao retirar o heroi da base");


    struct Evento impressao;

    impressao.ID_Base = Base->ID;
    impressao.ID_heroi = Heroi->ID;
    impressao.Tempo = Mundo->Relogio;
    impressao.Tipo = EV_Sai;
    impressao.ID_Missao = -1;
    imprime_sai(Mundo, &impressao);


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
    Mundo->ev_tratados++;
}

void Viaja(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base)
{

    if (Heroi->Vida == 0)
    return;   

    struct Base *base_origem = Heroi->Base_Atual;
    int id_base_origem = base_origem->ID;
    int x_origem = base_origem->Local.x;
    int y_origem = base_origem->Local.y;

    int x_destino = Base->Local.x;
    int y_destino = Base->Local.y;

    double D = sqrt((x_destino - x_origem) * (x_destino - x_origem) + (y_destino - y_origem) * (y_destino - y_origem));
    // acima, tem a distancia entre bases

    int duracao_viagem = D / Heroi->Velocidade;

    struct Evento ev_viaja;
    ev_viaja.ID_Missao = -1;
    ev_viaja.Tipo = EV_Viaja;
    ev_viaja.ID_heroi = Heroi->ID;
    ev_viaja.ID_Base = Base->ID;
    ev_viaja.Tempo = Mundo->Relogio + duracao_viagem;
    imprime_viaja(Mundo, &ev_viaja, id_base_origem, Base->ID, D);

    struct Evento *evento = malloc(sizeof(struct Evento));
    evento->ID_heroi = Heroi->ID;
    evento->ID_Base = Base->ID; //base de destino
    evento->Tipo = EV_Chega;
    evento->Tempo = Mundo->Relogio + duracao_viagem;
    evento->ID_Missao = -1;

    fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
}

void Morre(struct Mundo *Mundo, struct Heroi *Heroi, struct Base *Base, int id_missao)
{
    struct Evento ev_morre;
    ev_morre.ID_heroi = Heroi->ID;
    ev_morre.ID_Base = Base->ID;
    ev_morre.Tempo = Mundo->Relogio;
    ev_morre.ID_Missao = id_missao;
    ev_morre.Tipo = EV_Morre;

    imprime_morre(&ev_morre);

    Heroi->Vida = 0;
    cjto_retira(Base->Presentes, Heroi->ID);
    
   struct Evento *evento = malloc(sizeof(struct Evento));
    evento->ID_heroi = Heroi->ID;
    evento->ID_Base = Base->ID;
    evento->Tipo = EV_Avisa;
    evento->Tempo = Mundo->Relogio;
    evento->ID_Missao = -1;
    fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
    
    Avisa(Mundo, Base); 
    Mundo->total_mortes++;
    Mundo->ev_tratados++;
}

void Missao(struct Mundo *Mundo, struct Missao *Missao)
{
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

        double D = sqrt((Missao->Local.x - x_base) * (Missao->Local.x - x_base) + (Missao->Local.y - y_base) * (Missao->Local.y - y_base));

        matriz[i][0] = D;
        matriz[i][1] = Mundo->Bases[i]->ID;
        matriz[i][2] = cjto_contem(hab_base, Missao->Habilidades);
        cjto_destroi(hab_base);
    } 

/*acima, estou completando uma matriz de 10 inhas e 3 colunas. Cada linha representa cada base
* a primeira coluna tem a distancia entre a base e a missao 
* a segunda coluna tem o id da base
* a terceira coluna tem 1 ou 0, 1 sendo se a base tem as habilidades suficientes 
* para completar a missao e 0 se nao tem
 */ 

    Missao->tentativas++; //aumenta a qtd de tentaticas para imprimir depois

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

        if(heroi_que_vai_morrer == -1)
        {
            printf("Nao tem heroi disponivel para morrer aqui. Logo, essa base deve estar vazia !!debug");
            return;
        }

        Morre(Mundo, Mundo->Herois_vivos[heroi_que_vai_morrer], Mundo->Bases[base_heroi_morre], Missao->ID);
        remove_heroi(Mundo, base_heroi_morre, heroi_que_vai_morrer);
        // em remove_heroi já coloca a vida dele em 0, por isso chamo Morre primeiro
        
        incrementa_xp(Mundo, Mundo->Bases[base_heroi_morre]->ID);

        // imprime que a missao foi concluioda aqu
        struct Evento impressao;
        impressao.ID_Base = base_heroi_morre;
        impressao.ID_heroi = heroi_que_vai_morrer;
        impressao.ID_Missao = Missao->ID;
        impressao.Tempo = Mundo->Relogio;
        impressao.Tipo = EV_Missao;


        if(base_heroi_morre >= 0)
        {
            imprime_missao_concluida(Mundo, &impressao, impressao.ID_Base);
        }
        


        struct Evento *evento = malloc(sizeof(struct Evento));
        evento->ID_Base = matriz[0][1];
        evento->ID_heroi = -1;
        evento->Tipo = EV_Missao;
        evento->Tempo = Mundo->Relogio;
        evento->ID_Missao = Missao->ID;

        Mundo->Bases[base_heroi_morre]->n_missoes++;
        fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
        
    }
    if(bmp == -1 && (Mundo->Relogio % 2500 == 1))
    {
        
        struct Evento ev_missao_impo;
        ev_missao_impo.ID_Missao = Missao->ID;
        ev_missao_impo.Tempo = Mundo->Relogio;
        ev_missao_impo.ID_Base = -1;
        ev_missao_impo.Tipo = EV_Morre;
        ev_missao_impo.ID_heroi = -1;
        imprime_missao_impossivel(Mundo, &ev_missao_impo);
        //tem que fazer isso pois se chamar a função para imprimir a missao impossivel com 
        // as informações abaixo, o horario vai estar errado.

        struct Evento *evento = malloc(sizeof(struct Evento));
        evento->ID_Base = -1;
        evento->ID_heroi = -1;
        evento->Tipo = EV_Missao;
        evento->Tempo = Mundo->Relogio + (24 * 60);
        evento->ID_Missao = Missao->ID;
        
        Mundo->ev_tratados++;
        fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
    }
    if(bmp != -1)
    {
      
        incrementa_xp(Mundo, bmp);
      
          // chama função que imprime missao concluída 
        struct Evento impressao;
        impressao.ID_Base   = bmp;
        impressao.ID_heroi  = -1;
        impressao.ID_Missao = Missao->ID;
        impressao.Tempo     = Mundo->Relogio;
        impressao.Tipo      = EV_Missao;
        imprime_missao_concluida(Mundo, &impressao, bmp);


        struct Evento *evento = malloc(sizeof(struct Evento));
        evento->ID_Base = bmp;
        evento->ID_heroi = -1;
        evento->Tipo = EV_Missao;
        evento->Tempo = Mundo->Relogio;
        evento->ID_Missao = Missao->ID;

        Mundo->mi_cumpridas++;
        fprio_insere(Mundo->LEF, evento, evento->Tipo, evento->Tempo);
        Mundo->ev_tratados++;
        Mundo->Bases[bmp]->n_missoes++;
    }
    
    Mundo->Missoes[Missao->ID]->tentativas++;

}

void Fim(struct Mundo *Mundo)
{  
    imprime_fim(Mundo);
    fprio_destroi(Mundo->LEF);
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

void destroi_mundo(struct Mundo *Mundo)
{
  if(Mundo == NULL)
    return;

  //vamos destruir as bases
  for(int i = 0; i < Mundo->NBases; i++)
  {
    fila_destroi(Mundo->Bases[i]->Espera);
    cjto_destroi(Mundo->Bases[i]->Presentes);
    free(Mundo->Bases[i]);
  }
  free(Mundo->Bases);

  // vamos destruir herois
  for(int u = 0; u < Mundo->NHerois; u++)
  {
    cjto_destroi(Mundo->Herois_vivos[u]->Habilidades);
    free(Mundo->Herois_vivos[u]);
  }
  free(Mundo->Herois_vivos);

  //vamos destruir as missoes
  for(int w = 0; w < Mundo->NMissoes; w++)
  {
    cjto_destroi(Mundo->Missoes[w]->Habilidades);
    free(Mundo->Missoes[w]);
  }
  free(Mundo->Missoes);


  fprio_destroi(Mundo->LEF);
  free(Mundo);
}
