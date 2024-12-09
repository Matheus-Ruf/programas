#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lista.h"
#include "fprio.h"
#include "conjunto.h"
#include "boys.h"

//----------auxiliadores
int aleat(int min, int max)
{
    return min + rand() % (max - min + 1);
}

int distancia_cartesiana(struct coordenada *a, struct coordenada *b)
{
    int difx, dify;
    
    difx = b->x - a->x;
    dify = b->y - a->y;
    return sqrt( difx*difx + dify*dify );
}

int base_mais_perta(struct cjto_t *bases, struct missao *m, struct mundo *w)
{
    int base_perta, i;
    for(i = 0; i < w->nbases; i++)
    {
        if( i == 0
        || distancia_cartesiana((w->bases+i)->local, m->local) <
           distancia_cartesiana((w->bases+base_perta)->local, m->local) )
            base_perta = i;
    }
    return base_perta;
}

void imprime_missao(struct missao *m)
{
    printf("id: %d\n", m->id);
    printf("local{x:%d, y:%d}\n", m->local->x, m->local->y);
    printf("habs: ");cjto_imprime(m->habilidades);printf("\n");
    printf("perigo: %d\n", m->perigo);
    printf("status: %d\n", m->status);
    printf("tentativas: %d\n", m->tentativas);
    
    return;
}

void imprime_base(struct base *b)
{
    printf("id: %d\n", b->id);
    printf("lotacao: %d\n", b->lotacao);
    printf("presentes: ");cjto_imprime(b->presentes);printf("\n");
    printf("espera: ");fprio_imprime(b->espera);printf("\n");
    printf("local{x:%d, y:%d}\n", b->local->x, b->local->y);
    printf("fila_max: %d\n", b->fila_max);
    printf("missoes: %d\n", b->missoes);
    
    return;
}

void imprime_heroi(struct heroi *h)
{
    printf("id: %d\n", h->id);
    printf("habs: ");cjto_imprime(h->habilidades);printf("\n");
    printf("paciencia: %d\n", h->paciencia);
    printf("velocidade: %d\n", h->velocidade);
    printf("experiencia: %d\n", h->experiencia);
    printf("base: %d\n", h->base);
    printf("status: %d\n", h->status);
    
    return;
}

void imprime_mundo(struct mundo *w)
{
    printf("tamanhomundo{x:%d, y:%d}\n", w->tamanhomundo->x, w->tamanhomundo->y);
    printf("nhabilidades: %d\n", w->nhabilidades);
    printf("nherois: %d\n", w->nherois);
    printf("nbases: %d\n", w->nbases);
    printf("nmissoes: %d\n", w->nmissoes);
    printf("relogio: %d\n", w->relogio);
    printf("eventos tratados: %d\n", w->eventos_tratados);
    
    return;
}

//----------cria
struct missao cria_missao(int m_id, int b_x, int b_y, int nhabs, int per)
{
    struct missao m;
    
    m.local = malloc(sizeof(struct coordenada));
    if(!m.local)
    {
        return m;
    }
    
    m.id = m_id;
    m.local->x = b_x;
    m.local->y = b_y;
    m.habilidades = cjto_cria(nhabs);
    m.perigo = per;
    m.status = 0;
    m.tentativas = 0;
    
    return m;
}

struct base cria_base(int b_id, int b_x, int b_y, int lot)
{
    struct base b;
    
    b.local = malloc(sizeof(struct coordenada));
    if(!b.local)
    {
        return b;
    }
    
    b.id = b_id;
    b.local->x = b_x;
    b.local->y = b_y;
    b.lotacao = lot;
    b.presentes = cjto_cria(lot);
    b.espera = fprio_cria();
    b.fila_max = 0;
    b.missoes = 0;
    
    return b;
}

struct heroi cria_heroi(int h_id, int h_exp, int h_pac, int h_vel, int nhabs)
{
    struct heroi h;
    
    h.id = h_id;
    h.experiencia = h_exp;
    h.paciencia = h_pac;
    h.velocidade = h_vel;
    h.habilidades = cjto_cria(nhabs);
    h.status = 1;
    
    return h;
}

struct mundo *cria_mundo(int n_herois, int n_bases, int n_missoes, int n_habilidades, int n_tamanho_mundo, int t_incio)
{
    struct mundo *w;
    
    w = malloc(sizeof(struct mundo));
    if(!w)
        return NULL;
    
    w->tamanhomundo = malloc(sizeof(struct coordenada));
    if(!w->tamanhomundo)
    {
        free(w);
        return NULL;
    }
    
    w->nhabilidades = n_habilidades;
    w->nherois = n_herois;
    w->nbases = n_bases;
    w->nmissoes = n_missoes;
    
    w->herois = malloc(sizeof(struct heroi) * n_herois);
    if(!w->herois)
    {
        free(w->tamanhomundo);
        free(w);
        return NULL;
    }
    
    w->bases = malloc(sizeof(struct base) * n_bases);
    if(!w->bases)
    {
        free(w->herois);
        free(w->tamanhomundo);
        free(w);
        return NULL;
    }
    
    w->missoes = malloc(sizeof(struct missao) * n_missoes);
    if(!w->missoes)
    {
        free(w->bases);
        free(w->herois);
        free(w->tamanhomundo);
        free(w);
        return NULL;
    }
    w->tamanhomundo->x = n_tamanho_mundo;
    w->tamanhomundo->y = n_tamanho_mundo;
    w->relogio = t_incio;
    
    w->lef = fprio_cria();
    if(!w->lef)
    {
        free(w->missoes);
        free(w->bases);
        free(w->herois);
        free(w->tamanhomundo);
        free(w);
        return NULL;
    }
    
    w->eventos_tratados = 0;
    
    return w;
}


struct p_hbm *cria_phbm(struct heroi *h, struct base *b, struct missao *m)
{
    struct p_hbm *p3;
    
    p3 = malloc(sizeof(struct p_hbm));
    if(!p3)
        return NULL;
    
    p3->h = h;
    p3->b = b;
    p3->m = m;
    
    return p3;
}

void destroi_missao_sf(struct missao *m)
{
    m->habilidades = cjto_destroi(m->habilidades);
    free(m->local);
    m->local = NULL;
    
    return;
}

void destroi_base_sf(struct base *b)
{
    b->presentes = cjto_destroi(b->presentes);
    b->espera = fprio_destroi(b->espera);
    free(b->local);
    b->local = NULL;
    
    return;
}

void destroi_heroi_sf(struct heroi *h)
{
    h->habilidades = cjto_destroi(h->habilidades);
    
    return;
}

struct mundo *destroi_mundo(struct mundo *w)
{
    int i;
    
    //e os frees nas funcoes?
    for(i = w->nmissoes - 1; i >= 0; i--)
    {
        destroi_missao_sf(w->missoes+i);
    }
    
    for(i = w->nherois - 1; i >= 0; i--)
    {
        destroi_heroi_sf(w->herois+i);
    }
    
    for(i = w->nbases - 1; i >= 0; i--)
    {
        destroi_base_sf(w->bases+i);
    }
    //qualquer coisa bota destroi_algo_sf (sem free)
    
    free(w->missoes);
    w->missoes = NULL;
    free(w->herois);
    w->herois = NULL;
    free(w->bases);
    w->bases = NULL;
    free(w->tamanhomundo);
    w->tamanhomundo = NULL;
    
    w->lef = fprio_destroi(w->lef);
    
    free(w);
    w = NULL;
    
    return w;
}

//----------.h

/*
Representa um heroi H chegando em uma base B no instante T.
Ao chegar, o heroi analisa o tamanho da fila e decide se
espera para entrar ou desiste*/
void chega (int t, struct heroi *h, struct base *b, struct mundo *w)
{
    struct p_hbm *p3; 
    int espera, lixo;
    
    p3 = cria_phbm(h, b, NULL);
    if(!p3)
        return;
    
    h->base = b->id;
    
    espera = (cjto_card(b->presentes) < b->lotacao && fprio_tamanho(b->espera) == 0);
    if(!espera)
        espera = h->paciencia > (10 * fprio_tamanho(b->espera));
    
    if(espera)
    {
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n",
        t, h->id, b->id, b->presentes->num, b->lotacao);
        
        lixo = fprio_insere(w->lef, p3, 1, t);//espera(agora,h,b)
    }
    else
    {
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n",
        t, h->id, b->id, b->presentes->num, b->lotacao);
        
        lixo = fprio_insere(w->lef, p3, 2, t);//desiste(agora,h,b)
    }
    
    w->eventos_tratados++;
    return;
}


/*
O heroi H entra na fila de espera da base B.
Assim que H entrar na fila, o porteiro da base B deve
ser avisado para verificar a fila*/
void espera (int t, struct heroi *h, struct base *b, struct mundo *w)
{
    struct p_hbm *p3;
    int lixo;
    
    p3 = cria_phbm(h, b, NULL);
    if(!p3)
        return;
    
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n",
    t, h->id, b->id, fprio_tamanho(b->espera) );
    
    lixo = fprio_insere(b->espera, h, 0, 0);//insere h na fila de b;
    
    if(b->espera->num > b->fila_max)
        b->fila_max = b->espera->num;
    
    lixo = fprio_insere(w->lef, p3, 3, t);//avisa(agora,b)
    
    w->eventos_tratados++;
    return;
}


/*
O heroi H desiste de entrar na base B, escolhe uma base
aleatoria D e viaja para la*/
void desiste (int t, struct heroi *h, struct base *b, struct mundo *w)//struct mundo w
{
    struct p_hbm *p3;
    int lixo;
    
    p3 = cria_phbm(h, w->bases + aleat(0, w->nbases-1), NULL);
    if(!p3)
        return;
    
    printf("%6d: DESIST HEROI %2d BASE %d\n", t, h->id, b->id);
    lixo = fprio_insere(w->lef, p3, 6, t);//viaja(agora,h,d)
    
    w->eventos_tratados++;
    return;
}


/*
O porteiro da base B trata a fila de espera*/
void avisa (int t, struct base *b, struct mundo *w)
{
    struct fprio_t *copia;
    struct p_hbm *p3;
    int lixo;
    
    copia = fprio_cria();
    if(!copia)
        return;
    
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA [ ",
    t, b->id, b->presentes->num, b->lotacao);
    while( fprio_tamanho(b->espera) > 0 )
    {
        p3->h = fprio_retira(b->espera, &lixo, &lixo);
        printf("%2d ", p3->h->id);
        lixo = fprio_insere(copia, p3->h, 0, 0);
    }
    printf("]\n");
    
    while(cjto_card(b->presentes) < b->lotacao
      &&  cjto_card(b->presentes) != -1 
      &&  fprio_tamanho(b->espera) > 0)
    {
        p3 = cria_phbm(NULL, b, NULL);
        if(!p3)
            return;
        
        p3->h = fprio_retira(copia, &lixo, &lixo);
        
        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", t, b->id, p3->h->id);
        
        lixo = cjto_insere(b->presentes, p3->h->id);
        lixo = fprio_insere(w->lef, p3, 4, t);//entra(agora,h',b)
    }
    
    //devolve fila que restou, copia => b->espera
    while( fprio_tamanho(copia) > 0 )
    {
        lixo = fprio_insere( b->espera , fprio_retira(copia, &lixo, &lixo) , 0, 0 );
    }
    copia = fprio_destroi(copia);
    
    w->eventos_tratados++;
    return;
}


/*
O heroi H entra na base B. Ao entrar, o heroi decide
quanto tempo vai ficar e agenda sua saida da base*/
void entra (int t, struct heroi *h, struct base *b, struct mundo *w)
{
    struct p_hbm *p3;
    int lixo, tpb;
    
    p3 = cria_phbm(h, b, NULL);
    if(!p3)
        return;
    
    tpb = 15 + h->paciencia * aleat(1, 20);
    
    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n\n",
    t, h->id, b->id, b->presentes->num, b->lotacao, t + tpb);
    
    lixo = fprio_insere(w->lef, p3, 5, t + tpb);//sai(agora + tpb, h, b)
    
    w->eventos_tratados++;
    return;
}


/*
O heroi H sai da base B. Ao sair, escolhe uma base
de destino para viajar; o porteiro de B eh avisado,
pois uma vaga foi liberada*/
void sai (int t, struct heroi *h, struct base *b, struct mundo *w)
{
    struct p_hbm *p3;
    int lixo;
    
    p3 = cria_phbm(h, w->bases + aleat(0, w->nbases-1), NULL);
    if(!p3)
        return;
    
    lixo = cjto_retira(b->presentes, h->id);
    
    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n",
    t, h->id, b->id, b->presentes->num, b->lotacao);
    
    lixo = fprio_insere(w->lef, p3, 6, t);//viaja(agora,h,d)
    
    p3->b = b;
    lixo = fprio_insere(w->lef, p3, 3, t);//avisa(agora,b)
    
    w->eventos_tratados++;
    return;
}


/*
O heroi H se desloca para uma base D
(que pode ser a mesma onde ja esta)*/
void viaja (int t, struct heroi *h, struct base *d, struct mundo *w)
{
    struct p_hbm *p3;
    int lixo, distancia, duracao;
    
    p3 = cria_phbm(h, d, NULL);
    if(!p3)
        return;
    
    distancia = distancia_cartesiana((w->bases + h->base)->local, d->local);
    duracao = distancia / h->velocidade;
    
    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n",
    t, h->id, h->base, d->id, distancia, h->velocidade, t + duracao);
    //falta bastante coisa kk
    //veja como fica os frees do p_hb
    
    lixo = fprio_insere(w->lef, p3, 0, t + duracao);//chega(agora + duracao, h, d)
    
    w->eventos_tratados++;
    return;
}


/*
O heroi H morre no instante T.
 -O heroi eh retirado da base B e libera uma vaga na base.
 -O porteiro de B deve ser avisado da nova vaga.
 -Eventos futuros de um heroi morto devem ser ignorados.*/
void morre (int t, struct heroi *h, struct base *b, struct missao *m, struct mundo *w)
{
    struct p_hbm *p3;
    int lixo;
    
    p3 = cria_phbm(NULL, b, NULL);
    if(!p3)
        return;
    
    printf("%6d: MORRE  HEROI %2d MISSAO %d\n", t, h->id, m->id);
    
    lixo = cjto_retira(b->presentes, h->id);
    
    h->status = 0;//muda o status de h para morto
    
    lixo = fprio_insere(w->lef, p3, 3, t);//avisa(agora,b)
    
    w->eventos_tratados++;
    return;
}


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
void missao (int t, struct missao *m, struct mundo *w)
{
    struct p_hbm *p3;
    int bmp, risco;
    int i, j, k, lixo;
    
    //criar um conjunto que zera a cada base nova, colocar nesse conjunto
    //de um em um pegando das habilidades dos herois por vez e adicionando
    //a esse conjunto de habs de missao se ja nao estiver, faz isso e no fim
    //temos o conjunto completo
    
    struct cjto_t *cjto_habs, *cjto_habs_bmp, *cjto_bases;
    
    cjto_habs_bmp = NULL;
    bmp = -1;
    
    m->tentativas++;
    
    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", t, m->id, m->tentativas);
    cjto_imprime(m->habilidades);
    printf(" ]\n");
    
    //vamos fazer em ordem crescente de distancia da misssao
    //colocar as ids das bases em um cjto 
    cjto_bases = cjto_cria(w->nbases);
    for(i = 0; i < w->nbases; i++);
    {
        lixo = cjto_insere(cjto_bases, i);
    }
    
    
    i = base_mais_perta(cjto_bases, m, w);
    for(; cjto_bases->num > 0; i = base_mais_perta(cjto_bases, m, w))//para cada base
    {
        cjto_habs = cjto_cria(w->nhabilidades);
        if(!cjto_habs)
            return;
        
        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ",
        t, m->id, (w->bases + i)->id, distancia_cartesiana(m->local, (w->bases+i)->local) );
        cjto_imprime( (w->bases+i)->presentes );
        printf(" ]\n");
        
        //verifica todos herois
        for(j = 0; j < w->nherois; j++)//para cada heroi
        {
            if(cjto_pertence( (w->bases + i)->presentes, j ))//se o heroi esta na base i
            {
                printf("%6d: MISSAO %d HAB HEROI %2d: [ ", t, m->id, j);
                cjto_imprime( (w->herois+j)->habilidades );
                printf(" ]\n");
                
                //verifica todas habs
                for(k = 0; k < w->nhabilidades; k++)//para cada hab
                {
                    //verificar cada hab do mundo no conjunto
                    if(cjto_pertence( (w->herois+j)->habilidades, k ) == 1)//se h tem essa hab
                    {
                        if(cjto_pertence(cjto_habs, k) == 0)//se k nao pertence a cjto_habs
                            cjto_insere(cjto_habs, k);
                    }
                }
            }
        }
        
        printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", t, m->id, i);
        cjto_imprime(cjto_habs);
        printf(" ]\n");
        
        //testa se o cjto de hab da base contem cjto de habs da missao
        if(cjto_contem( cjto_habs, m->habilidades) == 1)
        {
            // "-1" no caso da primeira base encontrada
            if(bmp ==  -1 || distancia_cartesiana(m->local, (w->bases+i)->local)
                < distancia_cartesiana(m->local, (w->bases+bmp)->local ))//ver qual distancia eh menor
            {
                bmp = i;
                cjto_habs_bmp = cjto_destroi(cjto_habs_bmp);
                cjto_habs_bmp = cjto_copia(cjto_habs);
            }
        }
        
        cjto_habs = cjto_destroi(cjto_habs);
        lixo = cjto_retira(cjto_bases, i);
    }
    
    //se existe bmp
    if(bmp != -1)
    {
        m->status = 1;
        
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", t, m->id, bmp);
        cjto_imprime(cjto_habs_bmp);
        printf(" ]\n");
        
        (w->bases+bmp)->missoes++;
        
        for(i = 0; i < w->nherois; i++)
        {
            //se heroi i pertence a bmp
            if(cjto_pertence( (w->bases + bmp)->presentes, (w->herois + i)->id ))
            {
                risco = m->perigo / ( (w->herois+i)->paciencia + (w->herois+i)->experiencia + 1 );
                
                if(risco > aleat(0, 30))
                {
                    p3 = cria_phbm((w->herois + i), NULL, m);
                    if(!p3)
                        return;
                    
                    lixo = fprio_insere(w->lef, p3, 7, t);//morre(agora,h)
                }
                else
                {
                    (w->herois + i)->experiencia++;
                }
            }
        }
    }
    else
    {
        p3 = cria_phbm(NULL, NULL, m);
        if(!p3)
            return;
        
        printf("%6d: MISSAO %d IMPOSSIVEL\n", t, m->id);
        
        lixo = fprio_insere(w->lef, p3, 8, t + 24*60);//missao(t + 24*60,m)
    }
    
    w->eventos_tratados++;
    return;
}


/*
Encerra a simulação no instante T*/
void fim (int t, struct mundo *w)
{
    //w: eventos-tratados
    //m: tentativas
    
    int missoes_cumpridas, tentativas_min, tentativas_max, herois_mortos;
    float taxa_mortalidade, sucesso_missoes, media_tentativas;
    int i;
    //encerra a simulacao?
    //apresenta estatisticas de herois, bases e missoes
    
    printf("%6d: FIM\n", t);
    
    herois_mortos = 0;
    for(i = 0; i < w->nherois; i++)
    {
        if( (w->herois+i)->status == 0 )
        {
            herois_mortos++;
            
            printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [ ",
            i, (w->herois+i)->paciencia, (w->herois+i)->velocidade, (w->herois+i)->experiencia);
            cjto_imprime( (w->herois+i)->habilidades );
            printf(" ]\n");
        }
        else
        {
            printf("HEROI %2d VIVO  PAC %3d VEL %4d EXP %4d HABS [ ",
            i, (w->herois+i)->paciencia, (w->herois+i)->velocidade, (w->herois+i)->experiencia);
            cjto_imprime( (w->herois+i)->habilidades );
            printf(" ]\n");
        }
    }
    
    for(i = 0; i < w->nbases; i++)
    {
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n",
        i, (w->bases+i)->lotacao, (w->bases+i)->fila_max, (w->bases+i)->missoes);
    }
    
    missoes_cumpridas = 0;
    media_tentativas = 0;
    for(i = 0; i < w->nmissoes; i++)
    {
        if( (w->missoes + i)->status == 1 )
            missoes_cumpridas++;
        
        if(i == 0 || (w->missoes + i)->tentativas < tentativas_min)
            tentativas_min = (w->missoes + i)->tentativas;
        
        if(i == 0 || (w->missoes + i)->tentativas > tentativas_max)
            tentativas_max = (w->missoes + i)->tentativas;
        
        media_tentativas += (w->missoes + i)->tentativas;
    }
    
    sucesso_missoes = (missoes_cumpridas / w->nmissoes) * 100;
    media_tentativas /= w->nmissoes;
    taxa_mortalidade = (herois_mortos / w->nherois) * 100;
    
    printf("EVENTOS TRATADOS: %d\n", w->eventos_tratados);
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n",
    missoes_cumpridas, w->nmissoes, sucesso_missoes);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n",
    tentativas_min, tentativas_max, media_tentativas);
    printf("TAXA MORTALIDADE: %1.f%%\n", taxa_mortalidade);
    
    return;
}

/*
---------coisas pra fazer:
*testar NULL em p_hb [nao parece ter dado erro]
*lef no w
*declaracoes.h
*declaracoes nas funcoes de variaveis
*h->morto/vivo e m->cumprida/nao-cumprida?
*p_hbm ou p3
*distancia_cartesiana()
*frees no p_hb [vai ser dado no theboys.c]
*if(!)NULL
*printfs dos eventos
*free em mallocs anteriores
*fim()
theboys.c
p_hbm em boys.c?
testar/debugar
deixar os codigos bonitos: theboys,boys,conjunto,fprio,lista
mudar senha do moodle
avisar o aurelio

*/
