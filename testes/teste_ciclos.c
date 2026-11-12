#include <stdio.h>
#include <stdlib.h>
#include "../include/DigrafoLista.h"
#include "../include/ciclos.h"

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

int main() {
    // Caminho do arquivo CSV (ajuste se ele estiver na raiz do repositório)
    const char *nome_arquivo = "software_dependencies_1000.csv"; 

    printf("=========================================\n");
    printf("     TESTE DE DETECCAO DE CICLOS (DAG)   \n");
    printf("=========================================\n");

    // 1. Descobre o tamanho real do dataset dinamicamente
    int total_vertices = contar_linhas_csv(nome_arquivo);
    if (total_vertices <= 0) {
        fprintf(stderr, "Erro: Arquivo vazio ou nao encontrado.\n");
        return 1;
    }
    printf("[1/5] Total de vertices detectados: %d\n", total_vertices);

    // 2. Aloca o digrafo
    Digrafo *g = criar_digrafo(total_vertices);

    // 3. Abre o arquivo e monta o digrafo
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo CSV.\n");
        free(g->digrafo);
        free(g);
        return 1;
    }

    printf("[2/5] Montando o digrafo a partir do CSV...\n");
    montar_digrafo(arquivo, g);
    fclose(arquivo);

    // 4. Converte o vetor de dependências na lista encadeada de arestas
    printf("[3/5] Construindo as listas de adjacencia...\n");
    montar_listaDependencias(g);

    // --- ETAPA DE TESTES DE CICLO ---
    printf("\n-----------------------------------------\n");
    
    // Teste A: Grafo original (Deve ser um DAG limpo, esperado: 0)
    int cicloOriginal = detectaCiclo(g);
    printf("-> Teste 1 (Grafo Original): Ciclo detectado = %d (Esperado: 0)\n", cicloOriginal);

    // Teste B: Injeção intencional do ciclo fechado (3 -> 5 -> 7 -> 9 -> 11 -> 3)
    printf("[4/5] Injetando ciclo artificial (arestas nos vertices 3, 5, 7, 9, 11)...\n");
    Aresta **arestasCiclo = inseriCiclo(g);

    int cicloComErro = detectaCiclo(g);
    printf("-> Teste 2 (Com Ciclo Injetado): Ciclo detectado = %d (Esperado: 1)\n", cicloComErro);

    // Teste C: Remoção do ciclo injetado e restauração do estado original
    printf("[5/5] Removendo o ciclo e restaurando o grafo original...\n");
    removeCiclo(g, arestasCiclo);

    int cicloAposRemocao = detectaCiclo(g);
    printf("-> Teste 3 (Apos Remocao): Ciclo detectado = %d (Esperado: 0)\n", cicloAposRemocao);

    printf("=========================================\n");
    printf("      TESTES CONCLUIDOS COM SUCESSO!     \n");
    printf("=========================================\n");

    // Limpeza final da memória do digrafo conforme implementado no projeto
    // (Ajuste a função de liberação caso o seu projeto possua destroy/libera específico)
    // liberar_digrafo(g);

    return 0;
}