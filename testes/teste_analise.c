#include <stdio.h>
#include <stdlib.h>
#include "../include/DigrafoLista.h"
#include "../include/analise.h"

// Função auxiliar para contar o número de linhas do CSV
int contar_linhas_csv(const char *filename) {
    FILE *arquivo = fopen(filename, "r");
    if (arquivo == NULL) return -1;

    char buffer[1024];
    int linhas = 0;

    if (fgets(buffer, sizeof(buffer), arquivo) == NULL) {
        fclose(arquivo);
        return 0;
    }

    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        if (buffer[0] != '\n' && buffer[0] != '\r' && buffer[0] != '\0') {
            linhas++;
        }
    }

    fclose(arquivo);
    return linhas;
}

int main() {
    const char *nome_arquivo = "software_dependencies_1000.csv";

    printf("=========================================\n");
    printf("       MODULO DE TELEMETRIA E ANALISE    \n");
    printf("=========================================\n");

    // Inicia o cronômetro para medir o processo de carga
    clock_t inicio_cron = iniciar_cronometro();

    // 1. Conta o tamanho
    int total_vertices = contar_linhas_csv(nome_arquivo);
    if (total_vertices <= 0) {
        fprintf(stderr, "Erro: Arquivo CSV vazio ou nao encontrado.\n");
        return 1;
    }
    printf("[1/3] Total de vertices identificados: %d\n", total_vertices);

    // 2. Aloca e monta o digrafo
    Digrafo *g = criar_digrafo(total_vertices);
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo CSV.\n");
        free(g->digrafo);
        free(g);
        return 1;
    }

    printf("[2/3] Montando digrafo e listas de adjacencia...\n");
    montar_digrafo(arquivo, g);
    fclose(arquivo);
    montar_listaDependencias(g);

    // Finaliza a medição do tempo
    double tempo_ms = finalizar_cronometro(inicio_cron);

    // 3. Calcula o consumo de memória em bytes
    size_t memoria_bytes = calcular_memoria_digrafo(g);
    double memoria_kb = (double)memoria_bytes / 1024.0;
    double memoria_mb = memoria_kb / 1024.0;

    // Exibição dos resultados no console
    printf("\n-----------------------------------------\n");
    printf(" >>> RESULTADOS DA TELEMETRIA <<<\n");
    printf("-----------------------------------------\n");
    printf(" * Tempo de Execucao (Carga + Montagem): %.2f ms\n", tempo_ms);
    printf(" * Consumo de Memoria Total: %zu bytes\n", memoria_bytes);
    printf("   - Equivalente em KB: %.2f KB\n", memoria_kb);
    printf("   - Equivalente em MB: %.2f MB\n", memoria_mb);
    printf("-----------------------------------------\n");

    printf("=========================================\n");
    printf("     TESTE DE TELEMETRIA FINALIZADO!     \n");
    printf("=========================================\n");

    return 0;
}