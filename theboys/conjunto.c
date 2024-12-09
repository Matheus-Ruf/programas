#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"

// Cria um conjunto vazio para armazenar até CAP valores
struct cjto_t *cjto_cria(int cap) {
    if (cap <= 0) return NULL;

    struct cjto_t *conj = malloc(sizeof(struct cjto_t));
    if (!conj) return NULL;

    conj->flag = calloc(cap, sizeof(bool));
    if (!conj->flag) {
        free(conj);
        return NULL;
    }

    conj->cap = cap;
    conj->num = 0;
    return conj;
}

// Destrói o conjunto, liberando a memória
struct cjto_t *cjto_destroi(struct cjto_t *c) {
    if (!c) return NULL;

    free(c->flag);
    free(c);
    return NULL;
}

// Cria e retorna uma cópia do conjunto
struct cjto_t *cjto_copia(struct cjto_t *c) {
    if (!c) return NULL;

    struct cjto_t *copia = cjto_cria(c->cap);
    if (!copia) return NULL;

    for (int i = 0; i < c->cap; i++) {
        copia->flag[i] = c->flag[i];
    }
    copia->num = c->num;
    return copia;
}

// Cria um conjunto preenchido com N itens aleatórios
struct cjto_t *cjto_aleat(int n, int cap) {
    if (n < 0 || cap <= 0 || n > cap) return NULL;

    struct cjto_t *conj = cjto_cria(cap);
    if (!conj) return NULL;

    for (int i = 0; i < n; i++) {
        int item;
        do {
            item = rand() % cap;
        } while (conj->flag[item]);

        conj->flag[item] = true;
        conj->num++;
    }
    return conj;
}

// Insere um item no conjunto
int cjto_insere(struct cjto_t *c, int item) {
    if (!c || item < 0 || item >= c->cap) return -1;

    if (!c->flag[item]) {
        c->flag[item] = true;
        c->num++;
    }
    return c->num;
}

// Retira um item do conjunto
int cjto_retira(struct cjto_t *c, int item) {
    if (!c || item < 0 || item >= c->cap) return -1;

    if (c->flag[item]) {
        c->flag[item] = false;
        c->num--;
    }
    return c->num;
}

// Informa a cardinalidade do conjunto
int cjto_card(struct cjto_t *c) {
    return c ? c->num : -1;
}

// Testa se o item está no conjunto
int cjto_pertence(struct cjto_t *c, int item) {
    if (!c || item < 0 || item >= c->cap) return -1;

    return c->flag[item] ? 1 : 0;
}

// Testa se os conjuntos são iguais
int cjto_iguais(struct cjto_t *c1, struct cjto_t *c2) {
    if (!c1 || !c2 || c1->cap != c2->cap) return -1;

    for (int i = 0; i < c1->cap; i++) {
        if (c1->flag[i] != c2->flag[i]) return 0;
    }
    return 1;
}

// Testa se c1 contém c2
int cjto_contem(struct cjto_t *c1, struct cjto_t *c2) {
    if (!c1 || !c2 || c1->cap != c2->cap) return -1;

    for (int i = 0; i < c2->cap; i++) {
        if (c2->flag[i] && !c1->flag[i]) return 0;
    }
    return 1;
}

// Retorna o conjunto união de c1 e c2
struct cjto_t *cjto_uniao(struct cjto_t *c1, struct cjto_t *c2) {
    if (!c1 || !c2 || c1->cap != c2->cap) return NULL;

    struct cjto_t *uniao = cjto_cria(c1->cap);
    if (!uniao) return NULL;

    for (int i = 0; i < c1->cap; i++) {
        uniao->flag[i] = c1->flag[i] || c2->flag[i];
        if (uniao->flag[i]) uniao->num++;
    }
    return uniao;
}

// Retorna o conjunto interseção de c1 e c2
struct cjto_t *cjto_inter(struct cjto_t *c1, struct cjto_t *c2) {
    if (!c1 || !c2 || c1->cap != c2->cap) return NULL;

    struct cjto_t *inter = cjto_cria(c1->cap);
    if (!inter) return NULL;

    for (int i = 0; i < c1->cap; i++) {
        inter->flag[i] = c1->flag[i] && c2->flag[i];
        if (inter->flag[i]) inter->num++;
    }
    return inter;
}

// Retorna o conjunto diferença c1 - c2
struct cjto_t *cjto_dif(struct cjto_t *c1, struct cjto_t *c2) {
    if (!c1 || !c2 || c1->cap != c2->cap) return NULL;

    struct cjto_t *dif = cjto_cria(c1->cap);
    if (!dif) return NULL;

    for (int i = 0; i < c1->cap; i++) {
        dif->flag[i] = c1->flag[i] && !c2->flag[i];
        if (dif->flag[i]) dif->num++;
    }
    return dif;
}

// Imprime o conteúdo do conjunto
void cjto_imprime(struct cjto_t *c) {
    if (!c) return;

    int first = 1;
    for (int i = 0; i < c->cap; i++) {
        if (c->flag[i]) {
            if (!first) printf(" ");
            printf("%d", i);
            first = 0;
        }
    }
}
