/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los temporizadores
 * suministra los servicios independientemente del hardware
 *
 * usa los servicos de hal_tiempo.h: 
 */
 
#include "drv_tiempo.h"
#include "hal_tiempo.h"

#define TODO 0	//pa que no de error de compilacion con el proyecto vacio, modicicar
static uint32_t hal_ticks2us = HAL_TICKS2US;

/**
 * nicializa el reloj y empieza a contar
 */
void drv_tiempo_iniciar(void){
	hal_ticks2us = hal_tiempo_iniciar_tick();
}

/**
 * tiempo desde que se inicio el temporizador en microsegundos
 */
Tiempo_us_t drv_tiempo_actual_us(void){
	  uint64_t ticks_actuales = hal_tiempo_actual_tick();
    
    // Convierte los ticks a microsegundos usando la constante hal_ticks2us
    Tiempo_us_t tiempo_us = ticks_actuales / hal_ticks2us;
    
    return tiempo_us;
}

/**
 * tiempo desde que se inicio el temporizador en milisegundos
 */
Tiempo_ms_t drv_tiempo_actual_ms(void){
		Tiempo_us_t tiempo_us = drv_tiempo_actual_us();
    
    // Convierte el tiempo a milisegundos (1 ms = 1000 us)
    Tiempo_ms_t tiempo_ms = tiempo_us / 1000;
    
    return tiempo_ms;
}

/**
 * retardo: esperar un cierto tiempo en milisegundos
 */
void drv_tiempo_esperar_ms(Tiempo_ms_t ms){
	Tiempo_ms_t tiempo_inicio = drv_tiempo_actual_ms();
    while ((drv_tiempo_actual_ms() - tiempo_inicio) < ms)
    {
        // Espera activa
    }
}

/**
 * esperar hasta un determinado tiempo (en ms), devuelve el tiempo actual
 */
Tiempo_ms_t drv_tiempo_esperar_hasta_ms(Tiempo_ms_t ms){
	Tiempo_ms_t tiempo_actual = drv_tiempo_actual_ms();
    
    // Esperar hasta que el tiempo actual sea mayor o igual al tiempo especificado
    while (tiempo_actual < ms) {
        // Actualizamos el tiempo actual
        tiempo_actual = drv_tiempo_actual_ms();
    }
    
    // Devuelve el tiempo actual
    return tiempo_actual;
}

/* *****************************************************************************
 * Activacion Periodica contador de ticks
 */
static uint32_t callback_id;
static void(*f_callback)();

static void interna_callback(void){
	f_callback(callback_id, drv_tiempo_actual_ms());
}

void drv_tiempo_periodico_ms(Tiempo_ms_t ms, void(*funcion_callback)(), uint32_t id) {
	callback_id = id;
	f_callback = funcion_callback;

	
    if (ms == 0) {
        // Si el periodo es 0, detener el temporizador
        hal_tiempo_reloj_periodico_tick(0, interna_callback);
    } else {
        // Convertir el periodo de ms a ticks
        uint32_t periodo_en_ticks = ms * hal_ticks2us * 1000;  // Conversión de ms a ticks
        hal_tiempo_reloj_periodico_tick(periodo_en_ticks, interna_callback);
    }
}
