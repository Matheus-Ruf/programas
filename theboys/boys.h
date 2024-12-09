#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "fprio.h"
#include "conjunto.h"

//---------------struct

struct p_hbm//ponteiro triplo para heroi, base e missao
{
    struct heroi *h;
    struct base *b;
    struct missao *m;
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
    struct fprio_t *espera;         //fila onde os herois esperam para poder entrar na base
    struct coordenada *local;   //localizacao da base (par de coordenadas inteiras X, Y >= 0)
    int fila_max;               //tamanho maximo em que a fila ja ficou
    int missoes;                //numero de missoes em que a base participou
};

struct missao
{
    int id;                           //numero inteiro >= 0 que identifica a missao
    struct cjto_t *habilidades;    //conjunto de habilidades necessarias para cumprir a missao
    int perigo;                //nivel de perigo da missao
    struct coordenada *local;      //localizacao da missao (par de coordenadas inteiras X, Y >= 0)
    int status;                     // nao cumprida == 0; cumprida == 1
    int tentativas;                 //numero de tentativas na missao
};

struct mundo
{
    int nherois;                                 //numero total de herois no mundo
    struct heroi *herois;                       //vetor representando todos os herois
    int nbases;                                //numero total de bases no mundo
    struct base *bases;                       //vetor representando todas as bases
    int nmissoes;                            //numero total de missoes a cumprir
    struct missao *missoes;                 //vetor representando todas as missoes
    int nhabilidades;                      //numero de habilidades distintas possiveis
    struct coordenada *tamanhomundo;      //coordenadas maximas do plano cartesiano (as coordenadas minimas sao 0, 0); vamos considerar que o mapa 
                                         //de nossa supercidade eh representado por um plano cartesiano de tamanho tal que cada unidade representa 1 metro;
    int relogio;                        //numero inteiro >= 0 indicando o tempo atual no mundo. Cada unidade de tempo no mundo simulado representa 1 minuto de tempo real
    struct fprio_t *lef;                //lista de eventos futuros
    int eventos_tratados;               //numero de eventos realizados
};


//--------------funcoes
int aleat(int min, int max);


void imprime_missao(struct missao *m);
void imprime_base(struct base *b);
void imprime_heroi(struct heroi *h);
void imprime_mundo(struct mundo *w);

struct missao cria_missao(int m_id, int b_x, int b_y, int nhabs, int per);
struct base cria_base(int b_id, int b_x, int b_y, int lot);
struct heroi cria_heroi(int h_id, int h_exp, int h_pac, int h_vel, int nhabs);
struct mundo *cria_mundo(int n_herois, int n_bases, int n_missoes, int n_habilidades, int n_tamanho_mundo, int t_incio);
struct p_hbm *cria_phbm(struct heroi *h, struct base *b, struct missao *m);

void destroi_missao_sf(struct missao *m);
void destroi_base_sf(struct base *b);
void destroi_heroi_sf(struct heroi *h);
struct mundo *destroi_mundo(struct mundo *w);


/*
Representa um heroi H chegando em uma base B no instante T.
Ao chegar, o heroi analisa o tamanho da fila e decide se
espera para entrar ou desiste*/
void chega (int t, struct heroi *h, struct base *b, struct mundo *w);

/*
O heroi H entra na fila de espera da base B.
Assim que H entrar na fila, o porteiro da base B deve
ser avisado para verificar a fila*/
void espera (int t, struct heroi *h, struct base *b, struct mundo *w);

/*
O heroi H desiste de entrar na base B, escolhe uma base
aleatoria D e viaja para la*/
void desiste (int t, struct heroi *h, struct base *b, struct mundo *w);

/*
O porteiro da base B trata a fila de espera*/
void avisa (int t, struct base *b, struct mundo *w);

/*
O heroi H entra na base B. Ao entrar, o heroi decide
quanto tempo vai ficar e agenda sua saida da base*/
void entra (int t, struct heroi *h, struct base *b, struct mundo *w);

/*
O heroi H sai da base B. Ao sair, escolhe uma base
de destino para viajar; o porteiro de B eh avisado,
pois uma vaga foi liberada*/
void sai (int t, struct heroi *h, struct base *b, struct mundo *w);

/*
O heroi H se desloca para uma base D
(que pode ser a mesma onde ja esta)*/
void viaja (int t, struct heroi *h, struct base *d, struct mundo *w);

/*
O heroi H morre no instante T.
 -O heroi eh retirado da base B e libera uma vaga na base.
 -O porteiro de B deve ser avisado da nova vaga.
 -Eventos futuros de um heroi morto devem ser ignorados.*/
void morre (int t, struct heroi *h, struct base *b, struct missao *m, struct mundo *w);

/*
Uma missao M eh disparada no instante T.
Sao caracteristicas de uma missao:
 -Cada missao ocorre em um local aleatorio e requer um conjunto
   aleatorio de habilidades; ambos sao definidos durante a inicializaçao.
 -Cada equipe eh formada pelo conjunto de herois presentes em uma base.
 -Uma equipe esta apta para a missao se a uniao das habilidades
   de seus herois contem as habilidades requeridas pela missao.
 -Deve ser escolhida para a missao a equipe da base mais proxima ao
   local da missao e que esteja apta para ela.
 -Ao completar uma missao, os herois da equipe escolhida ganham pontos
   de experiencia.
 -Um heroi pode morrer ao participar de uma missao.
 -Se uma missao nao puder ser completada, ela eh marcada como “impossivel”
   e adiada de 24 horas.*/
void missao (int t, struct missao *m, struct mundo *w);

/*
Encerra a simulação no instante T*/
void fim (int t, struct mundo *w);


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
