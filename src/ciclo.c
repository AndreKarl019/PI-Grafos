#include <stdio.h>
#include <stdlib.h>
#include "../include/ciclos.h" 

static int DFS_aux(Digrafo *g, int indiceVertice, int *estado) {
   
    estado[indiceVertice] = 1;
   
    No *atual = g->digrafo[indiceVertice].lista;
    while (atual != NULL) {
        int vizinhoIdx = atual->vertice; 

        if (estado[vizinhoIdx] == 1) {
            return 1; 
        }

        if (estado[vizinhoIdx] == 0) {
            if (DFS_aux(g, vizinhoIdx, estado)) {
                return 1; 
            }
        }

        atual = atual->proximo;
    }

    estado[indiceVertice] = 2;
    return 0;
}

int detectaCiclo(Digrafo *g) {
    if (!g || !g->digrafo) return 0;

    int numV = g->tamanho;

    int *estado = (int *) calloc(numV, sizeof(int));
    if (!estado) return 0;

    for (int i = 0; i < numV; i++) {
        if (estado[i] == 0) {
            if (DFS_aux(g, i, estado)) {
                free(estado);
                return 1; 
            }
        }
    }

    free(estado);
    return 0; 
}

Aresta **inseriCiclo(Digrafo *g) {
    if (!g || g->tamanho <= 11) return NULL; 

    Aresta **arestas = (Aresta **) malloc(5 * sizeof(Aresta *));
    if (!arestas) return NULL;

    int origem[5]  = {3, 5, 7, 9, 11};
    int destino[5] = {5, 7, 9, 11, 3};

    for (int i = 0; i < 5; i++) {
        arestas[i] = (Aresta *) malloc(sizeof(Aresta));
        arestas[i]->m = origem[i];
        arestas[i]->n = destino[i];

        No *x = (No *) malloc(sizeof(No));
        if (!x) {
            for (int j = 0; j <= i; j++) { free(arestas[j]); }
            free(arestas);
            return NULL;
        }
        x->vertice = destino[i];

        No *aux = g->digrafo[origem[i]].lista;
        g->digrafo[origem[i]].lista = x;
        x->proximo = aux;
    }

    return arestas; 
}

void removeCiclo(Digrafo *g, Aresta **arestas) {
    if (!g || !arestas) return;

    for (int i = 0; i < 5; i++) {
        if (arestas[i]) {
            int origem = arestas[i]->m;

            if (g->digrafo[origem].lista != NULL) {
                No *remover = g->digrafo[origem].lista;
                g->digrafo[origem].lista = g->digrafo[origem].lista->proximo;
                free(remover); 
            }

            free(arestas[i]);
        }
    }

    free(arestas);
}