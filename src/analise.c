#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/analise.h"

clock_t iniciar_cronometro() {
    return clock();
}

double finalizar_cronometro(clock_t inicio) {
    clock_t fim = clock();
    return ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC;
}

size_t calcular_memoria_digrafo(Digrafo *g) {
    if (!g) return 0;

    size_t total_bytes = 0;

    total_bytes += sizeof(Digrafo);

    total_bytes += g->tamanho * sizeof(Lista);

    for (int i = 0; i < g->tamanho; i++) {
        if (g->digrafo[i].nome != NULL) {
            total_bytes += strlen(g->digrafo[i].nome) + 1;
        }

        if (g->digrafo[i].dependencias != NULL) {
            total_bytes += g->digrafo[i].qtd_dependencias * sizeof(int);
        }

        No *atual = g->digrafo[i].lista;
        while (atual != NULL) {
            total_bytes += sizeof(No);
            atual = atual->proximo;
        }
    }

    return total_bytes;
}