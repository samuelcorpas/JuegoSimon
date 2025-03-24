#ifndef HAL_WDT_H
#define HAL_WDT_H

#include "ifdebug.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//Función que inicializa el WDT
void hal_WDT_iniciar(uint32_t sec);

//Función para alimentar el WDT
void hal_WDT_feed(void);
#endif
