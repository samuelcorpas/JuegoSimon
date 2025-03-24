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
#include "drv_leds.h"
#include "board.h"


#if LEDS_NUMBER > 0
	static const uint8_t led_list[LEDS_NUMBER] = LEDS_LIST;
#endif

/**
 * inicializa los led, los deja apagados y devuelve el numero de leds disponibles en la plataforma
 */
uint32_t drv_leds_iniciar(){
	#if LEDS_NUMBER > 0
		for (uint32_t i = 0; i < LEDS_NUMBER; ++i) 			{
			hal_gpio_sentido(led_list[i], HAL_GPIO_PIN_DIR_OUTPUT);
			drv_led_apagar(i+1);
		}
  #endif //LEDS_NUMBER > 0	
	
	return LEDS_NUMBER;  //definido en board_xxx.h en cada placa... 
}

/**
 * enciende el led id, si id es cero ... no enciende ninguno?, todos? decidis vosotros
 */
void drv_led_encender(uint32_t id){
	#if LEDS_NUMBER > 0
		if ((id <= LEDS_NUMBER) && (id >0)) hal_gpio_escribir(led_list[id-1], LEDS_ACTIVE_STATE);
  #endif //LEDS_NUMBER > 0		
}

/**
 * funcion complementaria a encender
 */
void drv_led_apagar(uint32_t id){
	#if LEDS_NUMBER > 0
		if ((id <= LEDS_NUMBER) && (id >0)) hal_gpio_escribir(led_list[id-1], ~LEDS_ACTIVE_STATE);
  #endif //LEDS_NUMBER > 0	
}

/**
 * conmuta el led de on a off y viceversa
 * primero consulta el estado y lo invierte
 */
void drv_led_conmutar(uint32_t id){
	#if LEDS_NUMBER > 0
		if ((id <= LEDS_NUMBER) && (id >0)){
			hal_gpio_escribir(led_list[id-1], ~hal_gpio_leer(led_list[id-1]));
		}
  #endif //LEDS_NUMBER > 0		
}

//otras???
