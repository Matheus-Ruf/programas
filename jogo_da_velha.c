#include <stdio.h>
#include <stdlib.h>

int verifica_velha(int v[])
{
	
}

void imprime_tab(int v[])
{
	
}

void adiciona_tab(int v[], int x)
{
	
}

int main ()
{
	int v[8];
	int i, rodada;
	
	while(1)
	{
		printf("você quer jogar?\n\t1.sim\t2.não\n");
		
		do{	scanf("%d", &i);
		}while(i != 1 || i != 2);
		if(i == 2)
			return 0;
		
		velha = 0;
		for(rodada = 1; rodada <= 9; rodada++)
		{
			if(velha == 1)
			{
				imprime_tab();
				printf("GANHOU!!");
				rodada = 10;
			}
			else
			{
				adiciona_tab(v, rodada % 2);
				velha = verifica_velha(v);
			}
		}
		printf("VELHA!! o jogo acabou!");
	}
}
