/* *****************************************************************************
 * P.H.2024: hal_tiempos, interface que nos independiza del hardware 
 */
 
#ifndef HAL_TIEMPO
#define HAL_TIEMPO
#define HAL_TICKS2US			15 							// funcionamos PCLK a 15 MHz de un total de 60 MHz CPU Clock

#include <stdint.h>

/**
 * configura e inicializa la cuenta de tiempo en ticks del hardware y
 * devuelve la constante hal_ticks2us,
 * hal_ticks2us permite pasar de los ticks del hardware a microsegundos
 * (tip, el driver lo necesitara para trabajar en us y ms de la app y hacer la conversion a ticks del hardware)
 */
uint32_t hal_tiempo_iniciar_tick(void);

/**
 * nos devuelve el numero total de ticks desde que se inicio la cuenta
 */
uint64_t hal_tiempo_actual_tick(void);

/**
 * TODO para la practica 3, no se si hacer hal propio...
 * programa una activacion periodica cada periodo_en_tick ticks de la maquina
 */
void hal_tiempo_reloj_periodico_tick(uint32_t periodo_en_tick, void(*funcion_callback)()); 

#endif
