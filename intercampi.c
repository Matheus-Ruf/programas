#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "onibus.h"

/*
struct onibus
{
 cjto *horarios
 cjto *locais
};
*/

//-como deve aparecer
qual ferramenta quer usar?
{
  cronograma(selecionar quais onibus)
  destino(selecionar de onde para onde)
  locais_de_cada_onibus(mostra os locais que cada onibus pode ir)
  localizacao_simulada(selecionar quais onibus)
}

cronograma
{
  selecione:
  (-1)-concluido
  (-2)-retirar
  (-3)-limpar
  (-4)-cancelar
  
  1-intercampi 1
  2-intercampi 2
  3-intercampi 3
  4-intercampi 4(extra)
  5-intercampi noturno
  6-intercampi ferias
    [selecionados]
}
