#ifndef ANALISE_H
#define ANALISE_H

#include <time.h>
#include "DigrafoLista.h"

clock_t iniciar_cronometro();
double finalizar_cronometro(clock_t inicio);
size_t calcular_memoria_digrafo(Digrafo *g);

#endif