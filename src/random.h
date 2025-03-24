#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

// Funci�n para inicializar la semilla del generador de n�meros aleatorios
void random_init(uint32_t seed);

// Funci�n para generar un n�mero aleatorio en un rango espec�fico
uint32_t random_generate(uint32_t min, uint32_t max);

#endif
