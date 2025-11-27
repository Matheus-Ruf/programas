#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "conjunto.h"

//---------------struct

struct p_hbm//ponteiro triplo para heroi, base e missao
{
    struct heroi *heroi;
    struct base *base;
    struct missao *missao;
};

struct coordenada
{
    int x;   //numero inteiro >= 0 que indica o eixo horizontal
    int y;  //numero inteiro >= 0 que indica o eixo vertical
};

struct heroi
{
    int id;                          //numero inteiro >= 0 que identifica unicamente o heroi
    struct cjto_t *habilidades;   //conjunto de habilidades que o heroi possui. Cada habilidade eh representada por um numero inteiro >= 0
    int paciencia;                 //numero inteiro >= 0 que indica quao paciente uma pessoa eh. Em nosso modelo, isso afeta as decisões de permanencia em bases e ﬁlas
    int velocidade;               //numero inteiro >= 0 indicando a velocidade de deslocamento de um heroi, que irah afetar o tempo de deslocamento entre as bases
    int experiencia;             //numero inteiro >= 0 que indica o numero de missoes em que o heroi ja participou
    int base;                   //ID da base onde o herói se encontra no momento
    int status;                 //morto == 0; vivo == 1
};

struct base
{
    int id;                         //numero inteiro >= 0 que identifica cada base
    int lotacao;                   //numero maximo de herois naquela base
    struct cjto_t *presentes;   //conjunto dos IDs dos herois que estao atualmente na base, constituem as equipes disponiveis para realizar missoes
    struct fila_t *espera;         //fila onde os herois esperam para poder entrar na base
    struct coordenada *local;   //localizacao da base (par de coordenadas inteiras X, Y >= 0)
    int max_fila;               //tamanho maximo em que a fila ja ficou
    int missoes;                //numero de missoes em que a base participou
};

struct missao
{
    int id;                           //numero inteiro >= 0 que identifica a missao
    struct cjto_t *habilidades;    //conjunto de habilidades necessarias para cumprir a missao
    struct coordenada *local;      //localizacao da missao (par de coordenadas inteiras X, Y >= 0)
    int status;                     // nao cumprida == 0; cumprida == 1
    int tentativas;                 //numero de tentativas na missao
};

struct mundo
{
    int nherois;                                 //numero total de herois no mundo
    struct heroi **herois;                       //vetor representando todos os herois
    int nbases;                                //numero total de bases no mundo
    struct base **bases;                       //vetor representando todas as bases
    int nmissoes;                            //numero total de missoes a cumprir
    struct missao **missoes;                 //vetor representando todas as missoes
    int nhabilidades;                      //numero de habilidades distintas possiveis
    int ncompostosv;								//numero de compostos v disponıveis
    struct coordenada *tamanhomundo;      //coordenadas maximas do plano cartesiano (as coordenadas minimas sao 0, 0); vamos considerar que o mapa 
                                         //de nossa supercidade eh representado por um plano cartesiano de tamanho tal que cada unidade representa 1 metro;
    int relogio;                        //numero inteiro >= 0 indicando o tempo atual no mundo. Cada unidade de tempo no mundo simulado representa 1 minuto de tempo real
    struct fprio_t *lef;                //lista de eventos futuros
    int eventos_tratados;               //numero de eventos realizados
    int missoes_cumpridas;					//numero de missoes cumpridas
};

//------------aux
int aleat(int min, int max);
void destroi_hbm(struct mundo *w, int nherois, int nbases, int nmissoes);

//---------------heroi

struct heroi *cria_heroi(int id, int tamanho_cjto, int n_habs, int paciencia, int velocidade, int experiencia, int base, int status);

void imprime_heroi(struct heroi *h);

void destroi_heroi(struct heroi *h);

void insere_heroi_vet(struct heroi **vet, struct heroi *h, int pos);

//------------base

struct base *cria_base(int id, int lotacao, int n_herois, int coord_x, int coord_y);

void imprime_base(struct base *b);

void destroi_base(struct base *b);

void insere_base_vet(struct base **vet, struct base *b, int pos);

//---------------missao

struct missao *cria_missao(int id, int tamanho_cjto, int n_habs, int coord_x, int coord_y, int status, int tentativas);

void imprime_missao(struct missao *m);

void destroi_missao(struct missao *m);

void insere_missao_vet(struct missao **vet, struct missao *m, int pos);

//---------------mundo

struct mundo *cria_mundo(int nherois, int nbases, int nmissoes, int nhabilidades, int ncompostosv, int ntamanhomundo, int relogio, int eventos_tratados, int missoes_cumpridas);

void imprime_mundo(struct mundo *w);

void destroi_mundo(struct mundo *w, int nherois, int nbases, int nmissoes);

//--------------eventos
void chega(struct mundo *w, struct heroi *h, struct base *b, int t);

void espera(struct mundo *w, struct heroi *h, struct base *b, int t);

void desiste(struct mundo *w, struct heroi *h, int t);

void avisa(struct mundo *w, struct base *b, int t);

void entra(struct mundo *w, struct heroi *h, struct base *b, int t);

void sai(struct mundo *w, struct heroi *h, struct base *b, int t);

void viaja(struct mundo *w, struct heroi *h, struct base *d, int t);

void morre(struct mundo *w, struct heroi *h, struct base *b, struct missao *m, int t);

void missao(struct mundo *w, struct missao *m, int t);

void fim(struct mundo *w, int t);

/*

---struct
heroi: id, habilidades, paciencia, velocidade, experiencia, base.
base: id, lotacao, presentes, espera, local.
missao: id, habilidades, perigo, local.
mundo: nherois, herois, nbases, bases, nmissoes, missoes, nhabilidades,
       tamanhomundo, relogio.

---func/evento
chega(T,H,B)
espera(T,H,B)
desiste(T,H,B)
avisa(T,B)
entra(T,H,B)
sai(T,H,B)
viaja(T,H,D)
morre(T,H,B)
missao(T,M)
fim(T)

---siglas:
H = heroi
B = base
D = outra base
M = missao
W = mundo
T = tempo

*/
