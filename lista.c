#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
/* coloque demais includes aqui */

/* ------------------- Nao altere estas structs ------------------------- */
struct nodo {
    int chave;
    struct nodo *prox;
};


struct lista {
    struct nodo *ini;
    struct nodo *ptr; /* ponteiro para algum nodo da lista (iterador) */
    int tamanho;

};
/* ---------------------------------------------------------------------- */

/*
 * As funcoes abaixo implementam um iterador que vao permitir o usuario
 * de uma lista percorre-la, sem conhecer sua estrutura.
 *
 * Em conjunto, elas simulam um laço, sendo a inicializacao deste
 * a funcao lista_inicia_iterador e o incremento sendo a funcao
 * lista_incrementa_iterador.
 *
 * O ponteiro ptr da struct (iterador) eh inicializado apontando
 * para o primeiro elemento e incrementado ate' o ultimo elemento 
 * da lista.
 *
 * Ver exemplo de uso em testa_lista.c (funcao testa_imprimir_lista)
*/


/*
 * Inicializa ptr usado na funcao incrementa_iterador.
 * A funcao main deve garantir que a lista nao eh vazia.
*/
void lista_inicia_iterador (struct lista *lista)
{
	if(!lista)
		return;
	
	lista->ptr = lista->ini;
	return;
}


/*
 * Devolve no parametro *chave o elemento apontado e incrementa o iterador.
 * A funcao retorna 0 caso o iterador ultrapasse o ultimo elemento, ou retorna
 * 1 caso o iterador aponte para um elemento valido (dentro da lista).
*/
int lista_incrementa_iterador (struct lista *lista, int *chave)
{
	*chave = lista->ptr->chave;
	
	lista->ptr = lista->ptr->prox;
	if(!lista->ptr)
		return 0;
	return 1;
}


struct nodo *lista_ultima_posicao(struct lista *lista)
{
	if(!lista)
		return NULL;
	
	lista_inicia_iterador(lista);
	
	if(!lista->ptr)
		return NULL;
	
	while(lista->ptr->prox)
	{
		lista->ptr = lista->ptr->prox;
	}
	
	return lista->ptr;
}


struct nodo *lista_penultima_posicao(struct lista *lista)
{
	if(!lista)
		return NULL;
	
	lista_inicia_iterador(lista);
	
	if(!lista->ptr)
		return NULL;
	if(!lista->ptr->prox)
		return lista->ptr;
	
	while(lista->ptr->prox->prox)
	{
		lista->ptr = lista->ptr->prox;
	}
	
	return lista->ptr;
}


/*-----------------------------------------------------------------*/


/*
 * Cria e retorna uma nova lista.
 * Retorna NULL em caso de erro de alocação.
*/
struct lista *lista_cria ()
{
    struct lista *cria;
    
    cria = malloc(sizeof(struct lista));
    if(!cria)
    	return NULL;
    
    cria->ini = NULL;
    cria->ptr = NULL;
    cria->tamanho = 0;
    
    return cria;
}


/* Desaloca toda memoria da lista e faz lista receber NULL. */
void lista_destroi (struct lista **lista)
{
	int i, descarte;
	
	do
	{
		i = lista_remove_inicio(*lista, &descarte);
	}while(i != 0);
	
	free(*lista);
	*lista = NULL;
	
	return;
}


/*
 * Insere chave no inicio da lista. Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int lista_insere_inicio (struct lista *lista, int chave)
{
	struct nodo *novo;
	
	if(!lista)
		return 0;
	
	novo = malloc(sizeof(struct nodo));
	if(!novo)
		return 0;
	
	novo->chave = chave;
	novo->prox = lista->ini;
	
	lista->ini = novo;
	
	lista->tamanho++;
	
	return 1;
}


/*
 * Insere chave no final da lista. Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int lista_insere_fim (struct lista *lista, int chave)
{
	struct nodo *novo;
	
	if(!lista)
		return 0;
	
	novo = malloc(sizeof(struct nodo));
	if(!novo)
		return 0;
	
	novo->chave = chave;
	novo->prox = NULL;
	
	/*verifica se ha ultima posicao e coloca novo no fim*/
	lista->ptr = lista_ultima_posicao(lista);
	
	if(lista->ptr)/*coloca novo no fim*/
		lista->ptr->prox = novo;
	else/*coloca novo no fim/inicio*/
		lista->ini = novo;
	
	lista->tamanho++;
	
	return 1;
}


/*
 * Insere chave em ordem na lista. Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int lista_insere_ordenado (struct lista *lista, int chave)
{
	struct nodo *novo, *aux;
	int i, chave_da_lista;
	
	if(!lista)
		return 0;
	
	novo = malloc(sizeof(struct nodo));
	if(!novo)
		return 0;
	
	novo->chave = chave;
	novo->prox = NULL;
	
	/*
	inserir na ordem, pegar um nodo antes
	se tiver nodo menor e maior: inserir entre eles
	se n tiver nodo maior: inserir no fim
	se n tiver um nodo menor: inserir no inicio
	se n tiver nodo: inserir no inicio
	*/
	
	if(lista_vazia(lista))
	{
		free(novo);
		return lista_insere_inicio(lista, chave);
	}
	
	lista_inicia_iterador(lista);
	
	do
	{
		aux = lista->ptr;
		i = lista_incrementa_iterador(lista, &chave_da_lista);
	}while(chave_da_lista < chave || i == 0);
	
	if(i == 0)
	{
		free(novo);
		return lista_insere_fim(lista, chave);
	}
	
	if(aux == lista->ini)
	{
		free(novo);
		lista_insere_inicio(lista, chave);
	}
	
	novo->prox = aux->prox;
	aux->prox = novo;
	
	lista->tamanho++;
	
	return 1;
}


/*
 * Remove o elemento do inicio da lista e o retorna
 * no parametro chave. Nao confundir com o retorno da funcao.
 * A funcao retorna 1 em caso de sucesso e 0 no caso da lista estar vazia.
*/
int lista_remove_inicio (struct lista *lista, int *chave)
{
	if(!lista || !chave)
		return 0;
	
	if( lista_vazia(lista) )
		return 0;
	
	/*remove nodo da fila*/
	lista_inicia_iterador(lista);
	lista->ini = lista->ini->prox;
	
	/*libera o nodo*/
	*chave = lista->ptr->chave;
	lista->ptr->prox = NULL;
	free(lista->ptr);
	
	lista->ptr = NULL;
	
	lista->tamanho--;
	
	return 1;
}


/*
 * Remove o elemento do final da lista e o retorna
 * no parametro chave. Nao confundir com o retorno da funcao.
 * A funcao retorna 1 em caso de sucesso e 0 no caso da lista estar vazia.
*/
int lista_remove_fim (struct lista *lista, int *chave)
{
	int descarte;
	
	if(!lista || !chave)
		return 0;
	
	if( lista_vazia(lista) )
		return 0;
	
	/*remove o primeiro/ultimo no caso do tamanho da fila ser menos que dois, 1 se der certo, 2 se a fila estiver vazia*/
	if(lista_tamanho(lista) < 2)
		return lista_remove_inicio(lista, &descarte);
	
	lista->ptr = lista_penultima_posicao(lista);
	
	/*retira e libera o ultimo nodo*/
	*chave = lista->ptr->prox->chave;
	free(lista->ptr->prox);
	lista->ptr->prox = NULL;
	
	lista->tamanho--;
	
	return 1;
}


/*
 * Remove o elemento chave da lista mantendo-a ordenada.
 * A função considera que a cheve esta presente na lista, quem chama
 * esta funcao deve garantir isso.
 * A funcao retorna 1 em caso de sucesso e 0 no caso da lista estar vazia.
*/
int lista_remove_ordenado (struct lista *lista, int chave)
{
	struct nodo *aux;
	int i, chave_da_lista;
	
	if( lista_tamanho(lista) < 2)
		return lista_remove_inicio(lista, &chave_da_lista);
	
	lista_inicia_iterador(lista);
	
	do
	{
		aux = lista->ptr;
		i = lista_incrementa_iterador(lista, &chave_da_lista);
	}while(chave_da_lista == chave || i == 0);
	
	if(i == 0)
		return lista_remove_fim(lista, &chave_da_lista);
	
	aux->prox = aux->prox->prox;
	lista->ptr->prox = NULL;
	free(lista->ptr);
	
	lista->tamanho--;
	
	return 1;
}


/*
  Retorna 1 se a lista está vazia e 0 caso contrário.
*/
int lista_vazia (struct lista *lista)
{
	if( lista_tamanho(lista) == 0)
		return 1;
	return 0;
}


/*
  Retorna o numero de elementos da lista
*/
int lista_tamanho (struct lista *lista)
{
	return lista->tamanho;
}


/*
  Retorna 1 se o elemento chave esta presente na lista,
  caso contrário retorna 0.
*/
int lista_pertence (struct lista *lista, int chave)
{
	int i, chave_lista;
	
	if(!lista)
		return 0;
	
	if(lista_vazia(lista))
		i = 0;
	else
		i = 1;
	
	lista_inicia_iterador(lista);
	
	do
	{
		if(i == 0)/*nao ha o elemento na lista*/
			return 0;
		
		i = lista_incrementa_iterador(lista, &chave_lista);
	}while(chave_lista != chave);
	
	return 1;
}




