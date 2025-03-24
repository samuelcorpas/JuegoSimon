#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

// Función para inicializar la semilla del generador de números aleatorios
void random_init(uint32_t seed);

// Función para generar un número aleatorio en un rango específico
uint32_t random_generate(uint32_t min, uint32_t max);

#endif
