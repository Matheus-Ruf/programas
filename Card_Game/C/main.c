#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "cardgame.h"
//---------------------------------//--------------------------------------//main

int main()
{
	{	//teste ataque dano cria
		struct card *c1, *c2;

		c1 = cria_card();
		c2 = cria_card();

		escreve_card(c1);
		escreve_card(c2);

		imprime_card(c1);
		imprime_card(c2);

		while(c1->hp > 0 && c2->hp > 0)//vez de cada um no ataque
		{
			dano_geral( c1, c2 );
			if(c2->hp > 0)
				dano_geral( c2, c1 );
		}

		if(c1->hp <= 0)
			printf("card_%d perdeu todo o hp!!\n", c1->boneco);
		if(c2->hp <= 0)
			printf("card_%d perdeu todo o hp!!\n", c2->boneco);

		printf("\nFIM DE JOGO.\n");
		printf("resultado: card_%d:%dhp\tcard_%d:%dhp", c1->boneco, c1->hp, c2->boneco, c2->hp);
	}

	{	//teste escreve e imprime card
		/*
		    struct card *c;
		    c = malloc(sizeof(struct card));
		    if(!c)
		        return 1;

		    escreve_card(c);
		    printf("\n");
		    imprime_card(c);
		*/
	}

	{	//teste dado_d
		/*
		    int lados;
		    do{

		    do{
		        printf("L: ");
		        scanf("%d", &lados);
		    }while(lados <= 0 && lados != -10);

		    printf("dado: %d\n", dado_d(lados) );

		    }while(lados != 10);
		*/
	}

	return 0;
}
