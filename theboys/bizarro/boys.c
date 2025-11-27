#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
#include "boys.h"

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
//--------------------------auxiliares

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

struct coordenada *coord_cria(int x, int y)
{
	struct coordenada *coord;
	
	coord = malloc(sizeof(struct coordenada));
	if(!coord)
		return NULL;
	
	coord->x = x;
	coord->y = y;
	
	return coord;
}

void coord_imprime(struct coordenada *coord)
{
	if(!coord)
		return;
	printf("x: %d\ny: %d\n", coord->x, coord->y);
	return;
}

void coord_destroi(struct coordenada *coord)
{
	if(!coord)
		return;
	free(coord);
	coord = NULL;
	return;
}

int encontra_bmp(struct mundo *w, struct missao *m)
{
	struct base *b;
	int bmp, i;
	
	b = *(w->bases + 0);
	for(i = 0; i < w->nbases; i++)
	{
		if( distancia_cartesiana( m->local, b->local ) > distancia_cartesiana( m->local, (*(w->bases + i))->local ) )
			b = *(w->bases + i);
	}
	bmp = b->id;
	
	return bmp;
}

//0 = nao apta
//1 = apta
int base_apta(struct mundo *w, struct base *b, struct missao *m, int t, struct cjto_t *p_cjto_aux)
{
	int i, j;
	
	printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", t, m->id, b->id, distancia_cartesiana(m->local, b->local));
	cjto_imprime(b->presentes);
	printf(" ]\n");
	
	//cada heroi
	for(i = 0; i < w->nherois; i++)
	{
		//heroi da base
		if(cjto_pertence(b->presentes, i) && (*(w->herois + i))->status == 1)
		{
			printf("%6d: MISSAO %d HAB HEROI %2d: [ ", t, m->id, i);
			//cada habilidade
			for(j = 0; j < w->nhabilidades; j++)
			{
				//habilidade do heroi
				if(cjto_pertence( (*(w->herois + i))->habilidades, j ))
				{
					//adiciona habilidade do heroi as habilidades da base
					cjto_insere(p_cjto_aux, j);
					printf("%d ", j);
				}
			}
			printf("]\n");
		}
	}
	
	printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", t, m->id, b->id);
	cjto_imprime(p_cjto_aux);
	printf(" ]\n");
	
	if(cjto_contem(p_cjto_aux, m->habilidades))
		return 1;
	return 0;
}

//-1 se nao tiver
int encontra_bmp_apta(struct mundo *w, struct missao *m, int t, struct cjto_t *habilidades_da_base, struct cjto_t *p_cjto_aux)
{
	//ver as distancias e aptidao das bases
	//escolher a melhor
	struct base *b;
	int i, j;
	
	b = NULL;
	for(i = 0; i < w->nbases; i++)
	{
		if(base_apta(w, *(w->bases + i), m, t, p_cjto_aux))
		{
			if(b == NULL)
				b = *(w->bases + i);
			else
			{
				if(distancia_cartesiana( m->local, b->local ) > distancia_cartesiana( m->local, (*(w->bases + i))->local ))
					b = *(w->bases + i);
			}
		}
		if(b == *(w->bases + i))
		{
			for(j = 0; j < w->nhabilidades; j++)
				cjto_retira(habilidades_da_base, j);
			
			for(j = 0; j < w->nhabilidades; j++)
			{
				if(cjto_pertence(p_cjto_aux, j))
					cjto_insere(habilidades_da_base, j);
			}
		}
		
		for(j = 0; j < w->nhabilidades; j++)
				cjto_retira(p_cjto_aux, j);
	}
	
	if(b == NULL)
		return -1;
	
	return b->id;
}

void destroi_hbm(struct mundo *w, int nherois, int nbases, int nmissoes)
{
	int i;
	//liberar herois e vetor
	for(i = 0; i < nherois; i++)
	{
		destroi_heroi(*(w->herois + i));
		*(w->herois + i) = NULL;
	}
	free(w->herois);
	w->herois = NULL;
	
	//liberar bases e vetor
	for(i = 0; i < nbases; i++)
	{
		destroi_base(*(w->bases + i));
		*(w->bases + i) = NULL;
	}
	free(w->bases);
	w->bases = NULL;
	
	//liberar missoes e e vetor
	for(i = 0; i < nmissoes; i++)
	{
		destroi_missao(*(w->missoes + i));
		*(w->missoes + i) = NULL;
	}
	free(w->missoes);
	w->missoes = NULL;
	
	return;
}

//------------------------heroi

//tamanho_cjto: quantidade de habilidades para se ter no conjunto
//n_habs: as habilidades so serao aceitas se estiverem no intervalo de [0..tamanho_cjto - 1]
struct heroi *cria_heroi(int id, int tamanho_cjto, int n_habs, int paciencia, int velocidade, int experiencia, int base, int status)
{
	struct heroi *h;
	
	h = malloc(sizeof(struct heroi));
	if(!h)
		return NULL;
	
	h->habilidades = cjto_aleat(tamanho_cjto, n_habs);
	if(!h->habilidades)
	{
		free(h);
		h = NULL;
		return NULL;
	}
	
	h->id = id;
	h->base = base;
	h->status = status;
	h->paciencia = paciencia;
	h->velocidade = velocidade;
	h->experiencia = experiencia;
	
	return h;
}

void imprime_heroi(struct heroi *h)
{
	if(!h)
		return;
	printf("id: %d\nbase: %d\nstatus: %d\npaciencia: %d\nvelocidade: %d\nexperiencia: %d\nhabilidades: ", h->id, h->base, h->status, h->paciencia, h->velocidade, h->experiencia);
	cjto_imprime(h->habilidades);
	printf("\n");
}

void destroi_heroi(struct heroi *h)
{
	h->habilidades = cjto_destroi(h->habilidades);
	h->habilidades = NULL;
	free(h);
	h = NULL;
	return;
}

//*h pode ser null
//pos precisa ser maior igual a zero
//quem chamar deve garatir que vet eh acessivel
void insere_heroi_vet(struct heroi **vet, struct heroi *h, int pos)
{
	if(pos < 0)
		return;
	
	*(vet + pos) = h;
	return;
}

//-------------------base

struct base *cria_base(int id, int lotacao, int n_herois, int coord_x, int coord_y)
{
	struct base *b;
	
	b = malloc(sizeof(struct base));
	if(!b)
		return NULL;
	
	b->presentes = cjto_cria(n_herois);
	if(!b->presentes)
	{
		free(b);
		b = NULL;
		return NULL;
	}
	
	b->espera = fila_cria();
	if(!b->espera)
	{
		free(b->presentes);
		b->presentes = NULL;
		free(b);
		b = NULL;
		return NULL;
	}
	
	b->local = coord_cria(coord_x, coord_y);
	if(!b->local)
	{
		free(b->espera);
		b->espera = NULL;
		free(b->presentes);
		b->presentes = NULL;
		free(b);
		b = NULL;
		return NULL;
	}
	
	b->id = id;
	b->lotacao = lotacao;
	b->max_fila = 0;
	b->missoes = 0;
	
	return b;
}

void imprime_base(struct base *b)
{
	if(!b)
		return;
	
	printf("id: %d\nlotacao: %d\nmax_fila: %d\nmissoes: %d\n", b->id, b->lotacao, b->max_fila, b->missoes);
	coord_imprime(b->local);
	printf("presentes: ");
	cjto_imprime(b->presentes);
	printf("\nespera: ");
	fila_imprime(b->espera);
	printf("\n");
}

void destroi_base(struct base *b)
{
	if(!b)
		return;
	
	fila_destroi(b->espera);
	b->espera = NULL;
	cjto_destroi(b->presentes);
	b->presentes = NULL;
	coord_destroi(b->local);
	b->local = NULL;
	
	free(b);
	b = NULL;
	return;
}

//*b pode ser null
//pos precisa ser maior igual a zero
//quem chamar deve garatir que vet eh acessivel
void insere_base_vet(struct base **vet, struct base *b, int pos)
{
	if(pos < 0)
		return;
	
	*(vet + pos) = b;
	return;
}

//-----------------missao

struct missao *cria_missao(int id, int tamanho_cjto, int n_habs, int coord_x, int coord_y, int status, int tentativas)
{
	struct missao *m;
	
	m = malloc(sizeof(struct missao));
	if(!m)
		return NULL;
	
	m->habilidades = cjto_aleat(tamanho_cjto, n_habs);
	if(!m->habilidades)
	{
		free(m);
		m = NULL;
		return NULL;
	}
	
	m->local = coord_cria(coord_x, coord_y);
	if(!m->local)
	{
		free(m->habilidades);
		m->habilidades = NULL;
		free(m);
		m = NULL;
		return NULL;
	}	
	
	m->id = id;
	m->status = status;
	m->tentativas = tentativas;
	return m;
}

void imprime_missao(struct missao *m)
{
	if(!m)
		return;
	printf("id: %d\nstatus: %d\ntentativas: %d\nhabilidades: ",m->id, m->status, m->tentativas);
	cjto_imprime(m->habilidades);
	printf("\n");
	coord_imprime(m->local);
	return;
}

void destroi_missao(struct missao *m)
{
	if(!m)
		return;
	cjto_destroi(m->habilidades);
	m->habilidades = NULL;
	coord_destroi(m->local);
	m->local = NULL;
	free(m);
	m = NULL;
	return;
}

//*m pode ser null
//pos precisa ser maior igual a zero
//quem chamar deve garatir que vet eh acessivel
void insere_missao_vet(struct missao **vet, struct missao *m, int pos)
{
	if(pos < 0)
		return;
	
	*(vet + pos) = m;
	return;
}

//---------------------mundo

//vetor de ponteiros para herois, bases e missoes!!!!!
struct mundo *cria_mundo(int nherois, int nbases, int nmissoes, int nhabilidades, int ncompostosv, int ntamanhomundo, int relogio, int eventos_tratados, int missoes_cumpridas)
{
	struct mundo *w;
	int i;
	
	//alocacao
	w = malloc(sizeof(struct mundo));
	if(!w)
		return NULL;
	
	w->herois = malloc(sizeof(struct heroi *) * nherois);
	if(!w->herois)
	{
		free(w);
		w = NULL;
		
		return NULL;
	}
	
	w->bases = malloc(sizeof(struct base *) * nbases);
	if(!w->bases)
	{
		free(w->herois);
		w->herois = NULL;
		
		free(w);
		w = NULL;
		
		return NULL;
	}
	
	w->missoes = malloc(sizeof(struct missao *) * nmissoes);
	if(!w->missoes)
	{
		free(w->bases);
		w->bases = NULL;
		
		free(w->herois);
		w->herois = NULL;
		
		free(w);
		w = NULL;
		
		return NULL;
	}
	
	w->tamanhomundo = coord_cria(ntamanhomundo, ntamanhomundo);
	if(!w->tamanhomundo)
	{
		free(w->missoes);
		w->missoes = NULL;
		
		free(w->bases);
		w->bases = NULL;
		
		free(w->herois);
		w->herois = NULL;
		
		free(w);
		w = NULL;
		
		return NULL;
	}
	
	w->lef = fprio_cria();
	if(!w->lef)
	{
		free(w->tamanhomundo);
		w->tamanhomundo = NULL;
		
		free(w->missoes);
		w->missoes = NULL;
		
		free(w->bases);
		w->bases = NULL;
		
		free(w->herois);
		w->herois = NULL;
		
		free(w);
		w = NULL;
		
		return NULL;
	}
	
	//insercao dos inteiros
	w->nherois = nherois;
	w->nbases = nbases;
	w->nmissoes = nmissoes;
	w->nhabilidades = nhabilidades;
	w->ncompostosv = ncompostosv;
	w->relogio = relogio;
	w->eventos_tratados = eventos_tratados;
	w->missoes_cumpridas = missoes_cumpridas;
	
	//insercao de null nos ponteiros dos vetores das estruturas
	for(i = 0; i < nherois; i++)
		insere_heroi_vet(w->herois, NULL, i);
	for(i = 0; i < nbases; i++)
		insere_base_vet(w->bases, NULL, i);
	for(i = 0; i < nmissoes; i++)
		insere_missao_vet(w->missoes, NULL, i);
	
	return w;
}

void imprime_mundo(struct mundo *w)
{
	int i;
	
	if(!w)
		return;
	
	printf("-------------herois:\n");
	for(i = 0; i < w->nherois; i++)
	{
		imprime_heroi(*(w->herois + i));
		printf("\n");
	}
	
	printf("-------------bases:\n");
	for(i = 0; i < w->nbases; i++)
	{
		imprime_base(*(w->bases + i));
		printf("\n");
	}
	
	printf("-------------missoes:\n");
	for(i = 0; i < w->nmissoes; i++)
	{
		imprime_missao(*(w->missoes + i));
		printf("\n");
	}
	
	printf("-------------outros:\n");
	coord_imprime(w->tamanhomundo);
	printf("nherois: %d\n", w->nherois);
	printf("nbases: %d\n", w->nbases);
	printf("nmissoes: %d\n", w->nmissoes);
	printf("nhabilidades: %d\n", w->nhabilidades);
	printf("ncompostosv: %d\n", w->ncompostosv);
	printf("relogio: %d\n", w->relogio);
	printf("eventos_tratados: %d\n", w->eventos_tratados);
	printf("missoes_cumpridas: %d\n", w->missoes_cumpridas);
	
	printf("-------------lef:\n");
	fprio_imprime(w->lef);
	printf("\n");
	
	return;
}

void destroi_mundo(struct mundo *w, int nherois, int nbases, int nmissoes)
{	
	destroi_hbm(w, nherois, nbases, nmissoes);
	
	coord_destroi(w->tamanhomundo);
	
	w->lef = fprio_destroi(w->lef);
	
	free(w);
	w = NULL;
	
	return;
}

//--------------------------eventos

void chega(struct mundo *w, struct heroi *h, struct base *b, int t)
{
	struct p_hbm *evento;
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ", t, h->id, b->id, cjto_card(b->presentes), b->lotacao);
	
	h->base = b->id;
	evento->heroi = h;
	evento->base = b;
	
	//espera
	if((cjto_card(b->presentes) < b->lotacao && fila_tamanho(b->espera) == 0) || h->paciencia > (10 * fila_tamanho(b->espera)))
	{
		fprio_insere(w->lef, evento, ESPERA, t);
		printf("ESPERA\n");
	}
	else
	{
		//desiste
		fprio_insere(w->lef, evento, DESISTE, t);
		printf("DESISTE\n");
	}
	
	return;
}

void espera(struct mundo *w, struct heroi *h, struct base *b, int t)
{
	struct p_hbm *evento;
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", t, h->id, b->id, fila_tamanho(b->espera));
	
	fila_insere(b->espera, h->id);
	
	if(fila_tamanho(b->espera) > b->max_fila)
		b->max_fila = fila_tamanho(b->espera);
	evento->base = b;
	fprio_insere(w->lef, evento, AVISA, t);
	
	return;
}

void desiste(struct mundo *w, struct heroi *h, int t)
{
	struct p_hbm *evento;
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	printf("%6d: DESIST HEROI %2d BASE %d\n", t, h->id, h->base);
	
	evento->heroi = h;
	evento->base = *(w->bases + aleat(0, w->nbases-1));
	
	fprio_insere(w->lef, evento, VIAJA, t);
	
	return;
}

void avisa(struct mundo *w, struct base *b, int t)
{
	struct p_hbm *evento;
	int heroi_fila;
	
	printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", t, b->id, cjto_card(b->presentes), b->lotacao);
	fila_imprime(b->espera);
	printf(" ]\n");
	
	while(cjto_card(b->presentes) < b->lotacao && fila_tamanho(b->espera) > 0)
	{
		evento = malloc(sizeof(struct p_hbm));
		if(!evento)
			return;
		
		fila_retira(b->espera, &heroi_fila);
		cjto_insere(b->presentes, heroi_fila);
		
		evento->heroi = *(w->herois + heroi_fila);
		evento->base = b;
		
		fprio_insere(w->lef, evento, ENTRA, t);
		
		printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", t, b->id, heroi_fila);
	}
	
	return;
}

void entra(struct mundo *w, struct heroi *h, struct base *b, int t)
{
	struct p_hbm *evento;
	int tpb;//tempo de permanencia na base
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	tpb = 15 + h->paciencia * aleat(1,20);
	evento->heroi = h;
	evento->base = b;
	
	printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", t, h->id, b->id, cjto_card(b->presentes), b->lotacao, t + tpb);
	
	fprio_insere(w->lef, evento, SAI, t + tpb);
	
	return;
}

void sai(struct mundo *w, struct heroi *h, struct base *b, int t)
{
	struct p_hbm *evento;
	int base_d;
	
	cjto_retira(b->presentes, h->id);
	base_d = aleat(0, w->nbases-1);
	
	printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", t, h->id, b->id, cjto_card(b->presentes), b->lotacao);
	
	//viaja
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	evento->heroi = h;
	evento->base = *(w->bases + base_d);
	
	fprio_insere(w->lef, evento, VIAJA, t);
	
	//avisa
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	evento->base = *(w->bases + base_d);
	
	fprio_insere(w->lef, evento, AVISA, t);
	
	return;
}

void viaja(struct mundo *w, struct heroi *h, struct base *d, int t)
{
	struct p_hbm *evento;
	int distancia, duracao;
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	distancia = distancia_cartesiana( (*(w->bases + h->base))->local , d->local );
	
	if(h->velocidade <= 0)
		h->velocidade = 1;
	duracao = distancia / h->velocidade;
	
	printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", t, h->id, h->base, d->id, distancia, h->velocidade, t + duracao);
	
	evento->heroi = h;
	evento->base = d;
	
	fprio_insere(w->lef, evento, CHEGA, t + duracao);
	
	return;
}

void morre(struct mundo *w, struct heroi *h, struct base *b, struct missao *m, int t)
{
	struct p_hbm *evento;
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	printf("%6d: MORRE HEROI %2d MISSAO %d\n", t, h->id, m->id);
	
	cjto_retira(b->presentes, h->id);
	h->status = 0;
	
	evento->base = b;
	
	fprio_insere(w->lef, evento, AVISA, t);
	
	return;
}

void missao(struct mundo *w, struct missao *m, int t)
{
	struct p_hbm *evento;
	struct cjto_t *habilidades_da_base, *p_cjto_aux;
	int i, experiencia, bmp;//base mais proxima
	
	m->tentativas++;
	
	printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", t, m->id, m->tentativas);
	cjto_imprime(m->habilidades);
	printf(" ]\n");
	
	
	habilidades_da_base = cjto_cria(w->nhabilidades);
	if(!habilidades_da_base)
	{
		fprio_insere(w->lef, NULL, FIM, t);
		return;
	}
	
	p_cjto_aux = cjto_cria(w->nhabilidades);
	if(!p_cjto_aux)
	{
		cjto_destroi(habilidades_da_base);
		fprio_insere(w->lef, NULL, FIM, t);
		return;
	}
	
	bmp = encontra_bmp_apta(w, m, t, habilidades_da_base, p_cjto_aux);
	if(bmp >= 0)
	{
		m->status = 1;
		(*(w->bases + bmp))->missoes++;
		w->missoes_cumpridas++;
		
		for(i = 0; i < w->nherois; i++)
		{
			if(cjto_pertence( (*(w->bases + bmp))->presentes, i ))
				(*(w->herois + i))->experiencia++;
		}
		printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", t, m->id, bmp);
		cjto_imprime(habilidades_da_base);
		printf(" ]\n");
		
		cjto_destroi(habilidades_da_base);
		cjto_destroi(p_cjto_aux);
		habilidades_da_base = NULL;
		return;
	}
	else
	{
		cjto_destroi(habilidades_da_base);
		cjto_destroi(p_cjto_aux);
		habilidades_da_base = NULL;
		
		evento = malloc(sizeof(struct p_hbm));
		if(!evento)
			return;
		
		bmp = encontra_bmp(w, m);
		if(w->ncompostosv > 0 && t % 2500 == 0 && cjto_card((*(w->bases + bmp))->presentes) > 0)
		{
			w->ncompostosv--;
			m->status = 1;
			(*(w->bases + bmp))->missoes++;
			w->missoes_cumpridas++;
			
			experiencia = -1;
			for(i = 0; i < w->nherois; i++)
			{
				if(cjto_pertence( (*(w->bases + bmp))->presentes, i ) && (*(w->herois + i))->experiencia > experiencia)
				{
					experiencia = (*(w->herois + i))->experiencia;
					evento->heroi = *(w->herois + i);
				}
			}
			evento->base = *(w->bases + bmp);
			evento->missao = m;
			fprio_insere(w->lef, evento, MORRE, t);
			
			for(i = 0; i < w->nherois; i++)
			{
				if(cjto_pertence( (*(w->bases + bmp))->presentes, i ) && i != evento->heroi->id)
					(*(w->herois + i))->experiencia++;
			}
			return;
		}
		else
		{
			evento->missao = m;
			printf("%6d: MISSAO %d IMPOSSIVEL\n", t, m->id);
			fprio_insere(w->lef, evento, MISSAO, t + 24 * 60);
		}
		return;
	}
}

void fim(struct mundo *w, int t)
{
	int i, tentativas_max, tentativas_min, nmortos;
	float m_cumpridas_porcnt, n_mortos_porcnt, tentativas_media;
	
	printf("%6d: FIM\n\n", t);
	
	nmortos = 0;
	for(i = 0; i < w->nherois; i++)
	{
		printf("HEROI %2d ", i);
		if((*(w->herois + i))->status == 1)
			printf("VIVO");
		else
		{
			printf("MORTO");
			nmortos++;
		}
		printf(" PAC %3d VEL %4d EXP %4d HABS [ ", (*(w->herois + i))->paciencia, (*(w->herois + i))->velocidade, (*(w->herois + i))->experiencia);
		cjto_imprime((*(w->herois + i))->habilidades);
		printf(" ]\n");
	}
	printf("\n");
	
	for(i = 0; i < w->nbases; i++)
		printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", i, (*(w->bases + i))->lotacao, (*(w->bases + i))->max_fila, (*(w->bases + i))->missoes);
	printf("\n");
	
	printf("EVENTOS TRATADOS: %d\n", w->eventos_tratados);
	
	m_cumpridas_porcnt = (float) w->missoes_cumpridas / (float) w->nmissoes;
	m_cumpridas_porcnt *= 100;
	printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", w->missoes_cumpridas, w->nmissoes, m_cumpridas_porcnt);
	
	
	tentativas_max = (*(w->missoes + 0))->tentativas;
	tentativas_min = (*(w->missoes + 0))->tentativas;
	tentativas_media = 0;
	for(i = 0; i < w->nmissoes; i++)
	{
		if(tentativas_min > (*(w->missoes + i))->tentativas)
			tentativas_min = (*(w->missoes + i))->tentativas;
		if(tentativas_max < (*(w->missoes + i))->tentativas)
			tentativas_max = (*(w->missoes + i))->tentativas;
		tentativas_media += (*(w->missoes + i))->tentativas;
	}
	tentativas_media /= (float) w->nmissoes;
	
	printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", tentativas_min, tentativas_max, tentativas_media);
	
	n_mortos_porcnt = (float) nmortos / (float) w->nherois;
	n_mortos_porcnt *= 100;
	printf("TAXA MORTALIDADE: %.1f%%\n", n_mortos_porcnt);
	
	return;
}



/*
---------coisas pra fazer:

// n fazer 2 - imprimir em ordem as bases nas missoes
// n fazer 3 - parar de imprimir bases em missoes quando encontrar


6 - colocar comentarios
7 - separar em .c e .h coerentes
8 - deixar bonito

*/
