#include <stdio.h>
#include <stdlib.h>

int venceu(int v[])//venceu == 1; nao venceu == 0;
{
	int i, j;
	
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
			printf("\t");
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
    return;
}

void imprime_tab_3(int v[], int i)
{
	for(int j = 0; j < 3; j++)
		printf("[%d]", i + j + 1);
	return;
}

void imprime_tab_opcoes(int v[])
{
    int i;
    for(i = 0; i <= 8; i++)
    {
        if(i % 3 == 0)
            imprime_tab_3(v, i);
        
        if(v[i] == 2)
            printf("\t%d.", i+1);
        else
            printf("\t .");
            
        if(i % 3 == 2)
                printf("\n");
    }
    return;
}

void adiciona_tab(int v[], int jogador)// 0 (jogador 1) X ; 1 (jogador 2) O
{
    int i;
    
    imprime_tab_opcoes(v);
    
    printf("\n");
    do{
        if(jogador == 0)
         printf("jogador 1 (O): ");
        else
            printf("jogador 2 (X): ");
        
        scanf("%d", &i);
        i--;
    }while(0 > i || i > 8 || v[i] != 2); // 0 < i < 9; i nao repetido
    
    v[i] = jogador;
    
    return;
}

void reinicia(int v[])
{
    for(int i = 0; i <= 8; i++)
        v[i] = 2;
    return;
}

//O sempre comeca
int main ()
{
	int jogo[8];//[2,2,2,2,2,2,2,2]
	int i, rodada;

	while(1)//repete a cada vez que o jogo rodar
	{
		//escolhe se quer jogar
		printf("voce quer jogar?\n\t1.sim\t2.não\n");
		do{
		    printf("r: ");
		    scanf("%d", &i);
		}while(i != 1 && i != 2);
		if(i == 2)//se nao, acaba o laco
			return 0;
		
		printf("\n");
        
        reinicia(jogo);
        
		for(rodada = 0; rodada <= 8; rodada++)//sao 9 rodadas (repeticoes)
		{
			imprime_tab(jogo);
			printf("\n");
			
			adiciona_tab(jogo, rodada % 2);//resto pode ser 0 (jogador 1) ou 1 (jogador 2)
			printf("\n");
			
			if( venceu(jogo) )
			{
				printf("-------- GANHOU!! --------\n\n");
				rodada = 9;
			}
		}
		if( !venceu(jogo) )
			printf("--- VELHA!! o jogo acabou! ---\n\n");
		imprime_tab(jogo);
		printf("\n------------------------------\n\n");
	}
}
