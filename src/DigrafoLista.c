#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "digrafolista.h"

// Aloca o digrafo com um dado tamanho (número de vértices)
Digrafo *criar_digrafo(int tamanho) {
    Digrafo *g = (Digrafo *)malloc(sizeof(Digrafo));
    g->tamanho = tamanho;
    g->digrafo = (Lista *)malloc(tamanho * sizeof(Lista));

    for (int i = 0; i < tamanho; i++) {
        g->digrafo[i].vertice = i;
        g->digrafo[i].nome = NULL;
        g->digrafo[i].dependencias = NULL;
        g->digrafo[i].tempo_compilacao = 0.0f;
        g->digrafo[i].lista = NULL;
        g->digrafo[i].qtd_dependencias = 0;
    }

    return g;
}

// Insere n na vizinhança de m
void inserir_arco(Digrafo *g, int m, int n) {
    No *novo = (No *)malloc(sizeof(No));
    novo->vertice = n;
    novo->proximo = g->digrafo[m].lista;
    g->digrafo[m].lista = novo;
}

// Preenche os dados de um vértice específico no digrafo
void criar_no(Digrafo *g, int vertice, char *nome, int *dependencias, float tempo_compilacao, int qtd_deps) {
    int idx = vertice - 1;
    
    if (idx < 0 || idx >= g->tamanho) {
        fprintf(stderr, "Erro: Vértice %d fora dos limites do digrafo!\n", vertice);
        return;
    }

    if (g->digrafo[idx].nome != NULL) {
        fprintf(stderr, "Aviso/Erro: Vértice %d ('%s') duplicado no dataset!\n", vertice, nome);
        return; 
    }


    g->digrafo[idx].vertice = vertice;
    g->digrafo[idx].nome = strdup(nome);
    g->digrafo[idx].dependencias = dependencias;
    g->digrafo[idx].tempo_compilacao = tempo_compilacao;
    g->digrafo[idx].qtd_dependencias = qtd_deps;
}

void montar_dot(Digrafo *g, FILE *arquivo) {
    if (arquivo == NULL || g == NULL) return;

    fprintf(arquivo, "digraph G {\n");
    
    for (int i = 0; i < g->tamanho; i++) {
        if (g->digrafo[i].nome == NULL) continue;

        int vertice_id = g->digrafo[i].vertice; 
        char *nome = g->digrafo[i].nome;
        float tempo = g->digrafo[i].tempo_compilacao;

        fprintf(arquivo, "    node_%d [label=\"{ %s (id:%d) | %.2f ms }\", shape=Mrecord];\n", 
                vertice_id, nome, vertice_id, tempo);
    }

    fprintf(arquivo, "\n    // Conexões\n");

    for (int i = 0; i < g->tamanho; i++) {
        if (g->digrafo[i].nome == NULL) continue;

        int origem_id = g->digrafo[i].vertice; 
        No *atual = g->digrafo[i].lista;

        while (atual != NULL) {
            int destino_id = atual->vertice; 
            fprintf(arquivo, "    node_%d -> node_%d;\n", origem_id, destino_id);
            atual = atual->proximo;
        }
    }

    fprintf(arquivo, "}\n");
}

int *extrair_dependencias(char *str_deps, int *out_qtd) {
    *out_qtd = 0;
    if (!str_deps) return NULL;

    char *ptr = str_deps;
    int *deps = NULL;
    int count = 0;

    while (*ptr != '\0') {
        if (*ptr >= '0' && *ptr <= '9') {
            int num = 0;
            while (*ptr >= '0' && *ptr <= '9') {
                num = num * 10 + (*ptr - '0');
                ptr++;
            }
            
            int *temp = realloc(deps, (count + 1) * sizeof(int));
            if (!temp) {
                free(deps);
                return NULL;
            }
            deps = temp;
            deps[count++] = num;
        } else {
            ptr++;
        }
    }

    *out_qtd = count;
    return deps; 
}

// Lê o arquivo usando fscanf até o EOF e preenche o digrafo
void montar_digrafo(FILE *arquivo, Digrafo *g) {
    if (arquivo == NULL || g == NULL) return;
    
    int c;
    while ((c = fgetc(arquivo)) != '\n' && c != EOF);
    printf("Primeira linha removida\n");

    int posicao;
    char nome[256];
    char deps_str[512];
    float tempo_compilacao;

    char  aux_fim[16];

    while (fscanf(arquivo, "%d,%255[^,],%512[^]]]", &posicao, nome, deps_str) == 3) {
    // 1. O fscanf parou bem no ']'
    // 2. Agora precisamos avançar o ponteiro do arquivo para comer o que sobrou 
    //    (seja uma aspa e uma vírgula, ou apenas a vírgula) até chegar no número float.
    
    int ch;
    // Consome tudo até encontrar a vírgula (o que engole a aspa '"' extra se ela existir)
    while ((ch = fgetc(arquivo)) != ',' && ch != EOF);

    // 3. Agora que o arquivo está posicionado logo após a vírgula, lemos o float direto:
    if (fscanf(arquivo, "%f", &tempo_compilacao) != 1) {
        break; // Tratamento de erro caso o float falhe
    }

    // Consome a quebra de linha restante para preparar a próxima iteração
    while ((ch = fgetc(arquivo)) != '\n' && ch != EOF);

    int qtd_deps = 0;
    int *vetor_deps = extrair_dependencias(deps_str, &qtd_deps);

    criar_no(g, posicao, nome, vetor_deps, tempo_compilacao, qtd_deps);
}
}

void montar_listaDependencias(Digrafo *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->tamanho; i++) {

        for (int j = 0; j < g->digrafo[i].qtd_dependencias; j++) {
            inserir_arco(g, g->digrafo[i].vertice, g->digrafo[i].dependencias[j]);
        }
    }
}