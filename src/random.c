#include "random.h"

// Variables globales para almacenar el estado del generador (semilla)
static uint32_t seed = 0;

// Función para inicializar la semilla del generador de números aleatorios
void random_init(uint32_t init_seed) {
    if (init_seed != 0) {
        seed = init_seed;
    } else {
        seed = 123456789;  
    }
}

uint32_t random_generate(uint32_t min, uint32_t max) {
    seed = (1103515245 * seed + 12345) % (1 << 31);  // Generador congruencial lineal (LCG)
		uint32_t prueba = min + (seed % (max - min + 1));
    // Escalar el número aleatorio generado dentro del rango [min, max]
    return prueba;
}
