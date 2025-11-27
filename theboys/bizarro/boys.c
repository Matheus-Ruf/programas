//0 = nao apta
//1 = apta
int base_apta(struct mundo *w, struct base *b, struct missao *m, int t, struct cjto_t *habilidades_da_base)
{
	int i, j;
	
	printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", t, m->id, b->id, distancia_cartesiana(m->local, b->local));
	cjto_imprime(b->presentes);
	printf(" ]\n");
	
	//cada heroi
	for(i = 0; i < w->nherois; i++)
	{
		//heroi da base
		if(cjto_pertence(b->presentes, i) && (*(w->herois + i))->status == 1)
		{
			printf("%6d: MISSAO %d HAB HEROI %2d: [ ", t, m->id, i);
			//cada habilidade
			for(j = 0; j < w->nhabilidades; j++)
			{
				//habilidade do heroi
				if(cjto_pertence( (*(w->herois + i))->habilidades, j ))
				{
					//adiciona habilidade do heroi as habilidades da base
					cjto_insere(habilidades_da_base, j);
					printf("%d ", j);
				}
			}
			printf("]\n");
		}
	}
	
	printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", t, m->id, b->id);
	cjto_imprime(habilidades_da_base);
	printf(" ]\n");
	
	if(cjto_contem(habilidades_da_base, m->habilidades))
		return 1;
	return 0;
}
