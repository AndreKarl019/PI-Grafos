#ifndef CICLOS_H
#define CICLOS_H

#include "DigrafoLista.h"

typedef struct Aresta{
    int m;
    int n;
}Aresta;

Aresta ** inseriCiclo(Digrafo *g);
void removeCiclo(Digrafo *g, Aresta **m);
int detectaCiclo(Digrafo *g);

#endif
