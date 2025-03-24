#ifndef HAL_EXT_INT_H
#define HAL_EXT_INT_H

#include <stdint.h>
#include "rt_evento_t.h"

// Definiciones de las funciones a implementar por cada plataforma

//Iniciamos y habilitamos las interrupciones externas
void hal_ext_int_iniciar(uint32_t pin, uint32_t auxData, void (*callback)(uint32_t auxData));

//Habilitamos las interrupciones externas para ese pin
void hal_ext_int_habilitar_int(uint32_t pin);                

// Deshabilita la interrupción en el pin dado
void hal_ext_int_deshabilitar_int(uint32_t pin);     

// Habilita el despertar del sueño profundo por un pin
void hal_ext_int_habilitar_despertar(uint32_t pin);

// Deshabilita el despertar del sueño profundo por un pin
void hal_ext_int_deshabilitar_despertar(uint32_t pin); 

//Leemos si las interrupciones externas estan activas para ese determinado pin
uint32_t hal_ext_int_leerINT(uint32_t pin);

#endif // HAL_EXT_INT_H
