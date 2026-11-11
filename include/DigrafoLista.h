#ifndef DIGRAFOLISTA_H
#define DIGRAFOLISTA_H

#include <stdio.h>

typedef struct No{
    int vertice;
    struct No *proximo;
}No;

typedef struct Lista{
    int vertice;
    char* nome;
    int *dependencias;
    float tempo_compilacao;
    int qtd_dependencias;
    No *lista;
}Lista;

typedef struct Digrafo{
    int tamanho;
    Lista *digrafo;
}Digrafo;

Digrafo *criar_digrafo(int tamanho);
void inserir_arco(Digrafo *g, int m, int n);
void criar_no(Digrafo *g, int vertice, char *nome, int *dependencias, float tempo_compilacao, int qtd_deps);
void montar_dot(Digrafo *g, FILE *arquivo);
void montar_digrafo(FILE *arquivo, Digrafo *g);
void montar_listaDependencias(Digrafo *g);


#endif