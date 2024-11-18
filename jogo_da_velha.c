#include <stdio.h>
#include <stdlib.h>

int venceu(int v[])//venceu == 1; nao venceu == 0;
{
	for(i = 0; i < 2; i++)//verifica de cada jogador
	{
		for(j = 0; j < 9; j = j + 3)//verifica as linhas
		{
			if(v[j] == i)
			{
				if(v[j+1] == i)
				{
					if(v[j+2] == i)
						return 1;
				}
			}
		}
		
		for(j = 0; j < 3; j++)//verifica as colunas
		{
			if(v[j] == i)
			{
				if(v[j+3] == i)
				{
					if(v[j+6] == i)
						return 1;
				}
			}
		}
		for(j = 0; j < 3; j = j + 2)//verifica as diagonais
		{
			if( v[ 0 + j ] == i )
			{
				if( v[ 4 + (j-j) ] == i )
				{
					if( v[ 8 - j ] == i )
						return 1;
				}
			}
		}
	}
	return 0;
}

//	[0|1|2]
//	[3|4|5]
//	[6|7|8]
void imprime_tab(int v[])
{
	int i;
	for(i = 0; i <= 8; i++)
	{
		if(i % 3 == 0)
			printf("/t");
		printf("[");
		switch(v[i])
		{
			case 0:printf("O");break;
			case 1:printf("X");break;
			case 2:printf(" ");break;
		}
		printf("]");
		if(i % 3 == 2)
			printf("\n");
	}

}

void adiciona_tab(int v[], int x)
{

}

int main ()
{
	int jogo[2,2,2,2,2,2,2,2,2];//v[8]
	int i, rodada, velha;

	while(1)//repete a cada vez que o jogo rodar
	{
		//escolhe se quer jogar
		printf("você quer jogar?\n\t1.sim\t2.não\n");
		do{	scanf("%d", &i);
		}while(i != 1 || i != 2);
		if(i == 2)//se nao, acaba o laco
			return 0;

		for(rodada = 1; rodada <= 9; rodada++)//sao 9 rodadas (repeticoes)
		{
			imprime_tab();
			if( venceu(jogo) )
			{
				printf("GANHOU!!\n\n");
				rodada = 9;
			}
			else
			{
				adiciona_tab(jogo, rodada % 2);//resto pode ser 0 (jogador 1) ou 1 (jogador 2)
			}
		}
		if( !venceu(v) )
			printf("VELHA!! o jogo acabou!\n\n");
		//aqui tem que reiniciar o tabuleiro...
	}
}
