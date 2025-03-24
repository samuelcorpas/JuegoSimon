/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los Leds
 * suministra los servicios de iniciar, encender, apagar, conmutar... independientemente del hardware
 *
 * recoge de la definicion de la placa: 
 * LEDS_NUMBER - numero de leds existentes 
 * LEDS_LIST - array de leds que existen y sus gpios
 * el gpio de cada LED_x
 * LEDS_ACTIVE_STATE - si los leds son activos con el gpio a nivel alto o bajo
 *
 * Usa los servicios y tipos de datos del hal_gpio.h
 */

#include "hal_gpio.h"
#include "drv_monitor.h"
#include "board.h"


#if MONITOR_NUMBER > 0
	static const uint8_t monitor_list[MONITOR_NUMBER] = MONITOR_LIST;
#endif

/**
 * inicializa los monitores, los deja desmarcados y devuelve el numero de monitores disponibles en la plataforma
 */
uint32_t drv_monitor_iniciar(){
	#if MONITOR_NUMBER > 0
		for (uint32_t i = 0; i < MONITOR_NUMBER; ++i) 			{
			hal_gpio_sentido(monitor_list[i], HAL_GPIO_PIN_DIR_OUTPUT);
			drv_monitor_desmarcar(i+1);
		}
  #endif //MONITOR_NUMBER > 0	
	
	return MONITOR_NUMBER;  //definido en board_xxx.h en cada placa... 
}


/**
 * marca el monitor
 */
void drv_monitor_marcar(uint32_t id){
	#if MONITOR_NUMBER > 0
		if ((id <= MONITOR_NUMBER) && (id >0)) hal_gpio_escribir(monitor_list[id-1], MONITOR_ACTIVE_STATE);
  #endif //MONITOR_NUMBER > 0		

}

/**
 * desmarca el monitor
 */
void drv_monitor_desmarcar(uint32_t id){
	#if MONITOR_NUMBER > 0
		if ((id <= MONITOR_NUMBER) && (id >0)) hal_gpio_escribir(monitor_list[id-1], ~MONITOR_ACTIVE_STATE);
  #endif //MONITOR_NUMBER > 0	
}


//otras???
