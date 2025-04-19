#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "cardgame.h"

//---------------------------------//--------------------------------------//funcoes

//entrada: n > 0
//saida: 0 < int <= n
//devolve um valor aleatorio simulando um dado de n lados
int dado_d(int n)
{
	return rand() % n+1;
}

void escreve_card(struct card *c)
{
	printf("boneco: ");
	scanf("%d", &c->boneco);

	printf("hp: ");
	scanf("%d", &c->hp);

	printf("atkf: ");
	scanf("%d", &c->atkf);

	printf("atkm: ");
	scanf("%d", &c->atkm);

	printf("deff: ");
	scanf("%d", &c->deff);

	printf("defm: ");
	scanf("%d", &c->defm);

	printf("tipo: ");
	scanf("%d", &c->tipo);

	return;
}

void imprime_card(struct card *c)
{
	printf("card_%d || ", c->boneco);

	printf("hp:%d   ", c->hp);
	printf("atkf:%d   ", c->atkf);
	printf("atkm:%d   ", c->atkm);
	printf("deff:%d   ", c->deff);
	printf("defm:%d   ", c->defm);

	printf("tipo:");
	switch(c->tipo)
	{
	case 1:
		printf("morte");
		break;
	case 2:
		printf("sangue");
		break;
	case 3:
		printf("conhecimento");
		break;
	case 4:
		printf("energia");
		break;
	}
	printf("\n");

	return;
}

int ataque(struct card *c)//ataque magico ou fisico
{
	if(c->atkm == 0)
		return c->atkf;
	return c->atkm;
}

int dano_acerto(int ataque)//1 == errou; 2,3,4,5 = acertou; 6 == critico
{
	switch(dado_d(6))
	{
	case 1:
		printf("errou o ataque!!\n");
		return 0;
	case 2:
	case 3:
	case 4:
	case 5:
		printf("acertou o ataque!!\n");
		return ataque;
	}
    //case 6:
	printf("ataque critico!!\n");
	if(ataque % 2 == 0)
		return ataque * 1.5;
	return (ataque * 1.5) + 1;
}

int dano_adicional()    //dano de 1 a 6
{
	int da;
	da = dado_d(6);
	printf("d.ad:+%d   ", da);
	return da;
}

int dano_tipo(int tipo_atk, int tipo_def)
{
	int resultado;
	resultado = tipo_def - tipo_atk;
	/*
	    [ 1 - 1 = 0  ; 2 - 1 = 1  ; 3 - 1 = 2  ; 4 - 1 = 3 ]    [ nt ; vn ; nt ; dv ]
	    [ 1 - 2 = -1 ; 2 - 2 = 0  ; 3 - 2 = 1  ; 4 - 2 = 2 ]    [ dv ; nt ; vn ; nt ]
	    [ 1 - 3 = -2 ; 2 - 3 = -1 ; 3 - 3 = 0  ; 4 - 3 = 1 ]    [ nt ; dv ; nt ; vn ]
	    [ 1 - 4 = -3 ; 2 - 4 = -2 ; 3 - 4 = -1 ; 4 - 4 = 0 ]    [ vn ; nt ; dv ; nt ]
	*/
	if(resultado % 2 == 0)//tipos neutros (r = -2, 0, 2)
	{
		printf("d.tipo:+0    ");
		return 0;
	}
	if(resultado == -3 || resultado == 1)//tipo com vantagem (r = -3, 1)
	{
		printf("d.tipo:+2    ");
		return 2;
	}
    printf("d.tipo:-2    ");//tipo com desvantagem (r = -1, 3)
	return -2;
}

void dano_geral(struct card *c_atacante, struct card *c_receptor)
{
	int atk;

	atk = dano_acerto(ataque(c_atacante));//o ataque acertou?
	if(atk == 0)
		return;
	
	//soma de ataques
    printf("d.atk:+%d    ", atk);
	atk += dano_adicional();
	atk += dano_tipo(c_atacante->tipo, c_receptor->tipo);
	printf("\n");
    
    //resultado
	printf("card_%d -%d HP\n", c_receptor->boneco, atk);
	c_receptor->hp -= atk;
}

struct card *cria_card()//retorna ponteiro para a carta
{
	struct card *c;

	c = malloc(sizeof(struct card));
	if(!c)
		return NULL;

	return c;
}
