// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com lista encadeada simples

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria ()
{
	struct fprio_t *fila;
	
	fila = malloc(sizeof(struct fprio_t));
	if(!fila)
		return NULL;
	
	fila->prim = NULL;
	fila->num = 0;
	
	return fila;
}


// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f)
{
	void *lixo1;
	int *lixo2;
	
	lixo2 = malloc(sizeof(int));
	if(!f || !lixo2)
		return NULL;
	
	while(f->num != 0)
		lixo1 = fprio_retira(f, lixo2, lixo2);
	f->prim = NULL;
	
	free(f);
	f = NULL;
	
	free(lixo2);
	lixo2 = NULL;
	lixo1 = NULL;
	
	return lixo1;
}


// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio)
{
	struct fpnodo_t *inserido, *anterior;
	
	inserido = malloc(sizeof(struct fpnodo_t));
	if(!inserido || !f)
		return -1;
	
	inserido->item = item;
	inserido->tipo = tipo;
	inserido->prio = prio;
	
	if(f->num == 0 || inserido->prio < f->prim->prio)// insere no inicio
	{
		f->num++;
		inserido->prox = f->prim;
		f->prim = inserido;
		return f->num;
	}
	
	anterior = f->prim;
	while(anterior->prox && anterior->prox->prio <= inserido->prio)//anterior aponta para ultimo da fila com a prioridade menor
		anterior = anterior->prox;
	
	inserido->prox = anterior->prox;
	anterior->prox = inserido;
	
	f->num++;
	
	return f->num;
}


// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio)
{
	struct fpnodo_t *retirado;
	void *item;
	
	if(!f || f->num == 0 || !tipo || !prio)
		return NULL;
	
	f->num--;
	
	*tipo = f->prim->tipo;
	*prio = f->prim->prio;
	item = f->prim->item;
	
	retirado = f->prim;
	f->prim = f->prim->prox;
	retirado->prox = NULL;
	retirado->item = NULL;
	
	free(retirado);
	retirado = NULL;
	
	return item;
}


// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f)
{
	if(!f)
		return -1;
	return f->num;
}


// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f)
{
	struct fpnodo_t *apontador;
	int i;
	
	if(!f || f->num == 0)
		return;
	
	apontador = f->prim;
	for(i = 0; i < f->num; i++)
	{
		printf("(%d %d)", apontador->tipo, apontador->prio);
		if( !(apontador->prox == NULL) )
			printf(" ");
		apontador = apontador->prox;
	}
	
	return;
}
