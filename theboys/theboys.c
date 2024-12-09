// programa principal do projeto "The Boys - 2024/2"
// Autor: MATHEUS ALVAREZ RUFASTO, GRR 20231949

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "fprio.h"
#include "conjunto.h"
#include "boys.h"

// seus #defines vão aqui
#define T_INICIO 0//
#define T_FIM_DO_MUNDO 5//525600
#define N_TAMANHO_MUNDO 300//20000
#define N_HABILIDADES 3//10
#define N_HEROIS N_HABILIDADES*5//certo
#define N_BASES N_HEROIS/5//certo
#define N_MISSOES T_FIM_DO_MUNDO/100//certo
// minimize o uso de variáveis globais

// programa principal
int main ()
{
    //declarar variáveis
    struct mundo *w;
    struct p_hbm *p3, *p_aux;
    int i, ev, tempo, lixo;
    
    //inicializar mundo
    w = cria_mundo(N_HEROIS, N_BASES, N_MISSOES, N_HABILIDADES, N_TAMANHO_MUNDO, T_INICIO);
    
    //iniciando missoes
    for(i = 0; i < w->nmissoes; i++)
    {
        w->missoes[i] = cria_missao(i, aleat(0, N_TAMANHO_MUNDO - 1), aleat(0, N_TAMANHO_MUNDO - 1)
        , aleat(6, 10), aleat(0, 100));
    }
    
    //iniciando herois
    for(i = 0; i < w->nherois; i++)
    {
        w->herois[i] = cria_heroi(i, 0, aleat(0, 100), aleat(50, 5000), aleat(1, 3));
    }
    
    //iniciando bases
    for(i = 0; i < w->nbases; i++)
    {
        w->bases[i] = cria_base(i, aleat(0, N_TAMANHO_MUNDO - 1), aleat(0, N_TAMANHO_MUNDO - 1), aleat(3, 10));
    }
    
    //eventos iniciais
    
    //para cada heroi
    for(i = 0; i < w->nherois; i++)
    {
        (w->herois+i)->base = aleat(0, w->nbases - 1);
        tempo = aleat(0, 30);//aleat(0,4320)
        p3 = cria_phbm( w->herois + i, w->bases + (w->herois+i)->base, NULL);
        if(!p3)
            return -1;
        lixo = fprio_insere(w->lef, p3, 0, tempo);//chega(t,h,b)
    }
    
    //para cada missao
    for(i = 0; i < w->nmissoes; i++)
    {
        tempo = aleat(0, T_FIM_DO_MUNDO);
        p3 = cria_phbm(NULL, NULL, w->missoes + i);
        if(!p3)
            return -1;
        lixo = fprio_insere(w->lef, p3, 8, tempo);//missao(t,m)
    }
    
    tempo = T_FIM_DO_MUNDO;
    lixo = fprio_insere(w->lef, NULL, 9, tempo);
    
    // executar o laço de simulação
    
    w->relogio = 0;
    while(w->relogio < T_FIM_DO_MUNDO)
    {
        p_aux = fprio_retira(w->lef, &ev, &tempo);
        w->relogio = tempo;
        
        switch(ev)
        {
            case 0:
                chega(tempo, p_aux->h, p_aux->b, w);
                break;
            case 1:
                espera(tempo, p_aux->h, p_aux->b, w);
                break;
            case 2:
                desiste(tempo, p_aux->h, p_aux->b, w);
                break;
            case 3:
                avisa(tempo, p_aux->b, w);
                break;
            case 4:
                entra(tempo, p_aux->h, p_aux->b, w);
                break;
            case 5:
                sai(tempo, p_aux->h, p_aux->b, w);
                break;
            case 6:
                viaja(tempo, p_aux->h, p_aux->b, w);
                break;
            case 7:
                morre(tempo, p_aux->h, p_aux->b, p_aux->m, w);
                break;
            case 8:
                missao(tempo, p_aux->m, w);
                break;
            case 9:
                //fim do mundo
                break;
        }
        free(p_aux);
    }
    
    fim(tempo, w);
    
    // destruir o mundo
    w = destroi_mundo(w);
    
    return (0) ;
}

/*
    nhabs = 10
    nhero = 50
    nbase = 10
    nmiss = 5256
    
    nevet = 9
    tipos: 
        0 = chega
        1 = espera
        2 = desiste
        3 = avisa
        4 = entra
        5 = sai
        6 = viaja
        7 = morre
        8 = missao
        9 = fim
*/
