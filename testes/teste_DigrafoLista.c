#include <stdio.h>
#include <stdlib.h>
#include "digrafolista.h"

// Função auxiliar para contar o número real de linhas (vértices) no CSV
int contar_linhas_csv(const char *filename) {
    FILE *arquivo = fopen(filename, "r");
    if (arquivo == NULL) return -1;

    char buffer[1024];
    int linhas = 0;

    // Descarta a primeira linha (cabeçalho)
    if (fgets(buffer, sizeof(buffer), arquivo) == NULL) {
        fclose(arquivo);
        return 0;
    }

    // Conta o restante das linhas válidas do arquivo
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        if (buffer[0] != '\n' && buffer[0] != '\r' && buffer[0] != '\0') {
            linhas++;
        }
    }

    fclose(arquivo);
    return linhas;
}

// Função local para inspecionar o digrafo no terminal
void imprimir_digrafo_local(Digrafo *g) {
    if (g == NULL) return;

    printf("\n=========================================\n");
    printf("         INSPECAO LOCAL DO DIGRAFO       \n");
    printf("=========================================\n");

    for (int i = 0; i < 30; i++) {
        if (g->digrafo[i].nome == NULL) continue;

        printf("Vertice ID: %d\n", g->digrafo[i].vertice);
        printf("  - Nome: %s\n", g->digrafo[i].nome);
        printf("  - Tempo de Compilacao: %.2f ms\n", g->digrafo[i].tempo_compilacao);
        
        printf("  - Vetor de Deps (%d): [ ", g->digrafo[i].qtd_dependencias);
        for (int j = 0; j < g->digrafo[i].qtd_dependencias; j++) {
            printf("%d ", g->digrafo[i].dependencias[j]);
        }
        printf("]\n");

        printf("  - Lista de Arcos: ");
        No *atual = g->digrafo[i].lista;
        while (atual != NULL) {
            printf("%d -> ", atual->vertice);
            atual = atual->proximo;
        }
        printf("NULL\n");
        printf("-----------------------------------------\n");
    }
}

int main() {
    // Substitua pelo nome exato do seu arquivo CSV (ex: dependencies_1000.csv)
    const char *nome_arquivo = "software_dependencies_1000.csv"; 

    // 1. Descobre o tamanho real do dataset dinamicamente
    printf("1. Analisando o arquivo '%s'...\n", nome_arquivo);
    int total_vertices = contar_linhas_csv(nome_arquivo);
    
    if (total_vertices <= 0) {
        fprintf(stderr, "Erro: Arquivo vazio ou nao encontrado.\n");
        return 1;
    }
    
    printf("-> Total de vertices detectados no arquivo: %d\n", total_vertices);

    // 2. Aloca o digrafo com o tamanho exato extraído do arquivo
    Digrafo *g = criar_digrafo(total_vertices);

    // 3. Reabre o arquivo para a leitura dos dados via montar_digrafo
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao reabrir o arquivo.\n");
        free(g->digrafo);
        free(g);
        return 1;
    }

    printf("2. Lendo o arquivo e preenchendo o digrafo...\n");
    montar_digrafo(arquivo, g);
    fclose(arquivo);

    // 4. ESSENCIAL: Transforma o vetor de dependências na lista encadeada de arcos
    printf("3. Montando as listas de dependencias (arcos)...\n");
    montar_listaDependencias(g);

    // 5. Executa a inspeção visual de todos os nós no terminal
    imprimir_digrafo_local(g);

    // 6. Gera o arquivo .dot preenchido com os nós e as setas corretas
    printf("4. Gerando o arquivo 'grafo_saida.dot'...\n");
    FILE *dot_arquivo = fopen("grafo_saida.dot", "w");
    if (dot_arquivo != NULL) {
        montar_dot(g, dot_arquivo);
        fclose(dot_arquivo);
        printf("Sucesso! Arquivo .dot gerado com exito.\n");
    } else {
        fprintf(stderr, "Erro ao criar o arquivo .dot\n");
    }

    return 0;
}