// programa principal do projeto "The Boys - 2024/2"
// Autor: MATHEUS ALVAREZ RUFASTO, GRR 20231949

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
#include "boys.h"

// seus #defines vão aqui
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100
#define N_COMPOSTOS_V N_HABILIDADES * 3

#define CHEGA 0
#define ESPERA 1
#define DESISTE 2
#define AVISA 3
#define ENTRA 4
#define SAI 5
#define VIAJA 6
#define MORRE 7
#define MISSAO 8
#define FIM 9
// minimize o uso de variáveis globais

// programa principal
int main ()
{
    //declarar variáveis
    
    /*{
    struct heroi *h;
    int id, tamanho_cjto, n_habs, paciencia, velocidade, experiencia, base, status;
    
    id = 2;
    tamanho_cjto = 3;
    n_habs = 10;
    paciencia = 2;
    velocidade = 1;
    experiencia = 3;
    base = 1;
    status = 1;
    //struct p_hbm *p3, *p_aux;
    //int i, ev, tempo;
    
    h = cria_heroi(id, tamanho_cjto, n_habs, paciencia, velocidade, experiencia, base, status);
    imprime_heroi(h);
    destroi_heroi(h);
    }*/
    
    /*{
    struct base *b;
    int id, lotacao, n_herois, coord_x, coord_y;
    
    id = 5;
    lotacao = 12;
    n_herois = 21;
    coord_x = 12;
    coord_y = 0;
    
    b = cria_base(id, lotacao, n_herois, coord_x, coord_y);
    imprime_base(b);
    destroi_base(b);
    }*/
    
    /*{
    struct missao *m;
    int id, n_habs, perigo, coord_x, coord_y, status, tentativas;
    
    id = 2;
    n_habs = 7;
    perigo = 1;
    coord_x = 2;
    coord_y = 3;
    status = 0;
    tentativas = 21;
    
    m = cria_missao(id, n_habs, perigo, coord_x, coord_y, status, tentativas);
    imprime_missao(m);
    destroi_missao(m);
    }*/
    
    /*{
    	struct mundo *w;
    	int nherois, nbases, nmissoes, nhabilidades, ncompostosv, ntamanhomundo, relogio, eventos_tratados;
    	
    	int i;
    	struct heroi *h_aux;
    	struct base *b_aux;
    	struct missao *m_aux;
    	
    	//colocar var em h;
    	
    	//colocar var em b;
    	
    	//colocar var em m;
    	    	
    	nherois = 5;
    	nbases = 3;
    	nmissoes = 2;
    	nhabilidades = 6;
    	ncompostosv = 2;
    	ntamanhomundo = 50;
    	relogio = 0;
    	eventos_tratados = 0;
    	
    	w = cria_mundo(nherois, nbases, nmissoes, nhabilidades, ncompostosv, ntamanhomundo, relogio, eventos_tratados);
    	
    	//inicializa herois
    	for(i = 0; i < w->nherois; i++)
    	{
    		h_aux = cria_heroi(1,2,3,4,5,6,7,8);
    		insere_heroi_vet(w->herois, h_aux, i);
    	}
    	
    	//inicializa bases
    	for(i = 0; i < w->nbases; i++)
    	{
    		b_aux = cria_base(1,2,3,4,5);
    		insere_base_vet(w->bases, b_aux, i);
    	}
    	
    	//inicializa missoes
    	for(i = 0; i < w->nmissoes; i++)
    	{
    		m_aux = cria_missao(1,2,3,4,5,6,7);
    		insere_missao_vet(w->missoes, m_aux, i);
    	}
    	
    	imprime_mundo(w);
    	destroi_mundo(w);
    }*/
    
    
    //includes
    //func aux
    //main:
    ///declaracoes
    ///criacao/preparacao
    ///inicializacoes
    ///primeiros eventos
    
    struct mundo *w;
    struct heroi *h_aux;
    struct base *b_aux;
    struct missao *m_aux;
    struct p_hbm *evento;
    int i, tempo, tipo, tempo_evento;
    
    w = cria_mundo(N_HEROIS, N_BASES, N_MISSOES, N_HABILIDADES, N_COMPOSTOS_V, N_TAMANHO_MUNDO, T_INICIO, 0, 0);
    if(!w)
    	return 1;
    
    //inicializa herois
    for(i = 0; i < w->nherois; i++)
    {
    	h_aux = cria_heroi(i, aleat(1,3), N_HABILIDADES, aleat(0,100), aleat(50,5000), 0, 0, 1);
    	
    	if(!h_aux)
    	{
    		destroi_mundo(w, i, 0, 0);
    		return 1;
    	}
    	
    	insere_heroi_vet(w->herois, h_aux, i);
    }
    
    //inicializa bases
    for(i = 0; i < w->nbases; i++)
    {
    	b_aux = cria_base(i, aleat(3,10), N_HEROIS, aleat(0,N_TAMANHO_MUNDO-1), aleat(0,N_TAMANHO_MUNDO-1));
    	
    	if(!b_aux)
    	{
    		destroi_mundo(w, w->nherois, i, 0);
    		return 1;
    	}
    	
    	insere_base_vet(w->bases, b_aux, i);
    }
    
    //inicializa missoes
   for(i = 0; i < w->nmissoes; i++)
    {
    	m_aux = cria_missao(i, aleat(6,10), N_HABILIDADES, aleat(0,N_TAMANHO_MUNDO-1), aleat(0,N_TAMANHO_MUNDO-1), 0, 0);
    	
    	if(!m_aux)
    	{
    		destroi_mundo(w, w->nherois, w->nbases, i);
    		return 1;
    	}
    	
    	insere_missao_vet(w->missoes, m_aux, i);
    }
    
    //--eventos iniciais--
    
    //para cada heroi
    for(i = 0; i < w->nherois; i++)
    {
    	(*(w->herois + i))->base = aleat(0,N_BASES-1);
    	tempo = aleat(0,4320);
    	
    	evento = malloc(sizeof(struct p_hbm));
    	if(!evento)
    	{
    		while(fprio_tamanho(w->lef) > 0)
    		{
    			free(evento = fprio_retira(w->lef, &tipo, &tempo_evento));
   			}
    		destroi_mundo(w, w->nherois, w->nbases, w->nmissoes);
    		return 1;
    	}
    	evento->heroi = *(w->herois + i);
    	evento->base = *(w->bases + (*(w->herois + i))->base);
    	fprio_insere(w->lef, evento, CHEGA, tempo);
    }
    
    //para cada missao
    for(i = 0; i < w->nmissoes; i++)
    {
    	tempo = aleat(0, T_FIM_DO_MUNDO);
    	
    	evento = malloc(sizeof(struct p_hbm));
    	if(!evento)
    	{
    		while(fprio_tamanho(w->lef) > 0)
			{
    		free(evento = fprio_retira(w->lef, &tipo, &tempo_evento));
   			}
    		destroi_mundo(w, w->nherois, w->nbases, w->nmissoes);
    		return 1;
    	}
    	evento->missao = *(w->missoes + i);
    	fprio_insere(w->lef, evento, MISSAO, tempo);
    }
    
    //para fim do mundo
    evento = malloc(sizeof(struct p_hbm));//estou usando lixo de memoria
    if(!evento)
    {
    	while(fprio_tamanho(w->lef) > 0)
    	{
    		free(evento = fprio_retira(w->lef, &tipo, &tempo_evento));
   		}
   		destroi_mundo(w, w->nherois, w->nbases, w->nmissoes);
   		return 1;
    }
    tempo = T_FIM_DO_MUNDO;
    fprio_insere(w->lef, evento, FIM, tempo);
    
    //laco
    while(w->relogio < T_FIM_DO_MUNDO)
    {
    	evento = fprio_retira(w->lef, &tipo, &tempo_evento);
    	if(!evento)
    	{
    		while(fprio_tamanho(w->lef) > 0)
    		{
    			free(evento = fprio_retira(w->lef, &tipo, &tempo_evento));
   			}
   			destroi_mundo(w, w->nherois, w->nbases, w->nmissoes);
   			return 1;
   		 }
    	
    	switch(tipo)
    	{
    		case CHEGA:
    			chega(w, evento->heroi, evento->base, tempo_evento);
    			break;
    		
    		case ESPERA:
    			espera(w, evento->heroi, evento->base, tempo_evento);
    			break;
    		
    		case DESISTE:
    			desiste(w, evento->heroi, tempo_evento);
    			break;
    		
    		case AVISA:
    			avisa(w, evento->base, tempo_evento);
    			break;
    		
    		case ENTRA:
    			entra(w, evento->heroi, evento->base, tempo_evento);
    			break;
    		
    		case SAI:
    			if(evento->heroi->status == 1)
    				sai(w, evento->heroi, evento->base, tempo_evento);
    			break;
    		
    		case VIAJA:
    			viaja(w, evento->heroi, evento->base, tempo_evento);
    			break;
    		
    		case MORRE:
    			morre(w, evento->heroi, evento->base, evento->missao, tempo_evento);
    			break;
    		
    		case MISSAO:
    			missao(w, evento->missao, tempo_evento);
    			break;
    		
    		case FIM:
    			fim(w, tempo_evento);
    			break;
    		
    		default:
    			break;
    	}
    	free(evento);
    	w->eventos_tratados++;
    	w->relogio = tempo_evento;
    }
    
    //da free nos p_hbm que sobraram
    while(fprio_tamanho(w->lef) > 0)
    {
    	free(evento = fprio_retira(w->lef, &tipo, &tempo_evento));
   	}
    
    destroi_mundo(w, w->nherois, w->nbases, w->nmissoes);
    
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
