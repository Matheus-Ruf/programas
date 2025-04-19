#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

//---------------------------------//--------------------------------------//struct

struct card
{
	//char player[32];     //nome do player
	int boneco;           //indica qual das 32 cartas eh
	int hp;              //vida da carta, 0 <= hp <= 100 , se chegar a "0" a carta morre
	int atkf;           //ataque fisico, 0 <= atkf <= 20
	int atkm;          //ataque magico, 0 <= atkm <= 20
	int deff;         //defesa fisica, 0 <= deff <= 10
	int defm;        //defesa magica, 0 <= defm <= 10
	int tipo;       //tipo da carta, podendo ser: | 1 = morte | 2 = sangue | 3 = conhecimento | 4 = energia |
};

//---------------------------------//--------------------------------------//funcoes

//entrada: n > 0
//saida: 0 < int <= n
//devolve um valor aleatorio simulando um dado de n lados
int dado_d(int n);
void escreve_card(struct card *c);
void imprime_card(struct card *c);
int ataque(struct card *c);//ataque magico ou fisico
int dano_adicional();    //dano de 1 a 6
int dano_tipo(int tipo_atk, int tipo_def);
void dano_geral(struct card *c_atacante, struct card *c_receptor);
struct card *cria_card();//retorna ponteiro para a carta
