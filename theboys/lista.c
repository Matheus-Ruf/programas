// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

// Cria uma lista vazia.
// Retorno: ponteiro p/ a lista ou NULL em erro.
struct lista_t *lista_cria ()
{
	struct lista_t *cria;
	
	cria = malloc(sizeof(struct lista_t));
	if(!cria)
		return NULL;
	
	cria->tamanho = 0;
	cria->prim = NULL;
	cria->ult = NULL;
	
	return cria;
}

// Remove todos os itens da lista e libera a memória.
// Retorno: NULL.
struct lista_t *lista_destroi (struct lista_t *lst)
{
	int *item;
	
	item = malloc(sizeof(int));
	if(!item)
		return NULL;
	
	while(lst->tamanho > 0)
		lst->tamanho = lista_retira(lst, item, 0);

	free(lst);
	lst = NULL;
	
	return lst;
}

// Nas operações insere/retira/consulta/procura, a lista inicia na
// posição 0 (primeiro item) e termina na posição TAM-1 (último item).

// Insere o item na lista na posição indicada;
// se a posição for além do fim da lista ou for -1, insere no fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_insere (struct lista_t *lst, int item, int pos)
{
	struct item_t *adicionado;
	int i;
	
	adicionado = malloc(sizeof(struct item_t));
	if(!adicionado || !lst)
		return -1;
	
	if(pos < -1)
		return -1;
	
	adicionado->valor = item;
	adicionado->ant = NULL;
	adicionado->prox = NULL;
	
	if(lst->tamanho == 0)//insercao inicial
	{
	    lst->prim = adicionado;
	    lst->ult = adicionado;
	    
	    lst->tamanho++;
	    return lst->tamanho;
	}
	
	if(pos == -1 || pos >= lst->tamanho)//insere no fim
	{
		lst->ult->prox = adicionado;
		adicionado->ant = lst->ult;
		lst->ult = lst->ult->prox;
		
		lst->tamanho++;
		return lst->tamanho;
	}
	
	adicionado->prox = lst->prim;
	
	for(i = 0; i < pos; i++)//adicionado->prox aponta para o prox verdadeiro
		adicionado->prox = adicionado->prox->prox;
	
	adicionado->ant = adicionado->prox->ant;
	
	if(adicionado->prox->ant)   //se nao for inserido no inicio
	    adicionado->ant->prox = adicionado;
	else                        //se for inserido no inicio
	    lst->prim = adicionado;
	
	adicionado->prox->ant = adicionado;
	
	lst->tamanho++;
	return lst->tamanho;
}

// Retira o item da lista da posição indicada.
// se a posição for -1, retira do fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_retira (struct lista_t *lst, int *item, int pos)
{
	struct item_t *descartado;
	int i;
	
	if(pos >= lst->tamanho || pos < -1 || !lst || !item || lst->tamanho == 0)
		return -1;

	if(lst->tamanho == 1)//retira o ultimo
	{
		descartado = lst->prim;
		
		lst->prim = NULL;
		lst->ult = NULL;
		
		*item = descartado->valor;
		free(descartado);

		lst->tamanho--;
		return lst->tamanho;
	}

	if(pos == 0 )//retira do inicio
	{
		descartado = lst->prim;
		
		lst->prim = lst->prim->prox;
		
		*item = descartado->valor;
		descartado->prox = NULL;
		free(descartado);

		lst->tamanho--;
		return lst->tamanho;
	}
	
	if(pos == -1 )//retira do fim
	{
		descartado = lst->ult;
		
		lst->ult = lst->ult->ant;
		
		*item = descartado->valor;
		descartado->ant = NULL;
		free(descartado);

		lst->tamanho--;
		return lst->tamanho;
	}
	
	descartado = lst->prim;
	
	for(i = 0; i < pos; i++)//descartado aponta para o item que vamos retirar
		descartado = descartado->prox;
	
	descartado->ant->prox = descartado->prox;
	descartado->prox->ant = descartado->ant;
	
	*item = descartado->valor;
	descartado->ant = NULL;
	descartado->prox = NULL;
	free(descartado);

	lst->tamanho--;
	return lst->tamanho;
}

// Informa o valor do item na posição indicada, sem retirá-lo.
// se a posição for -1, consulta do fim.
// Retorno: número de itens na lista ou -1 em erro.
int lista_consulta (struct lista_t *lst, int *item, int pos)
{
	struct item_t *consultado;
	int i;
	
	if(pos < -1 || pos >= lst->tamanho || !lst || !item || lst->tamanho == 0)
		return -1;
	
	if(pos == -1)//consulta do fim
	{
		*item = lst->ult->valor;
		return lst->tamanho;
	}
	
	consultado = lst->prim;
	for(i = 0; i < pos; i++)//consultado aponta para o item que vamos consultar
		consultado = consultado->prox;
	
	*item = consultado->valor;
	return lst->tamanho;
}

// Informa a posição da 1ª ocorrência do valor indicado na lista.
// Retorno: posição do valor ou -1 se não encontrar ou erro.
int lista_procura (struct lista_t *lst, int valor)
{
	struct item_t *procurador;
	int posicao;
	
	procurador = lst->prim;
	for(posicao = 0; posicao < lst->tamanho; posicao++)//procurador anda pela lista e verifica
	{
		if(procurador->valor == valor)
			return posicao;
		procurador = procurador->prox;
	}
	
	return -1;
}

// Informa o tamanho da lista (o número de itens presentes nela).
// Retorno: número de itens na lista ou -1 em erro.
int lista_tamanho (struct lista_t *lst)
{
	if(!lst)
		return -1;
	return lst->tamanho;
}

// Imprime o conteúdo da lista do inicio ao fim no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois, sem newline.
void lista_imprime (struct lista_t *lst)
{
	struct item_t *pos;
	int i;
	
	if(!lst)
	    return;
	
	pos = lst->prim;
	for(i = 1; i <= lst->tamanho; i++)//anda pela lista
	{
	    printf("%d", pos->valor);
	    if(i < lst->tamanho)//sempre que nao for a ultima posicao
	        printf(" ");
	    pos = pos->prox;
	}
	return;
}
