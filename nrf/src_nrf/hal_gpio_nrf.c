/* *****************************************************************************
 * P.H.2024: TODO
 */
 
//#include "hal_gpio_nrf.h"
#include <nrf.h>
#include "stdint.h"
#include "hal_gpio.h"
#include "board_nrf52840dk.h"


/**
 * Permite emplear el GPIO y debe ser invocada antes
 * de poder llamar al resto de funciones de la biblioteca.
 * re-configura todos los pines como de entrada (para evitar cortocircuitos)
 */
void hal_gpio_iniciar(void){
	NRF_GPIO->DIR = 0;
}


/* *****************************************************************************
 * Acceso a los GPIOs 
 *
 * optimizado para campos/datos de mas de un bit
 * gpio_inicial indica el primer bit con el que operar.
 * num_bits indica cuántos bits con los que queremos operar. 
 */

/**
 * Los bits indicados se configuran como
 * entrada o salida según la dirección.

 */
void hal_gpio_sentido_n(uint32_t gpio_inicial, 
			uint8_t num_bits, hal_gpio_pin_dir_t direccion){

	uint32_t i;
    for (i = 0; i < num_bits; i++) {
        uint32_t pin = gpio_inicial + i;

        if (direccion == HAL_GPIO_PIN_DIR_INPUT) {
            // Configura el pin como entrada sin pull (equivalente a nrf_gpio_cfg_input)
            NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                     (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                     (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                     (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) |
																			(GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) | 
																			//(GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos) |
																			(GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);
        } else if (direccion == HAL_GPIO_PIN_DIR_OUTPUT) {
            // Configura el pin como salida (equivalente a nrf_gpio_cfg_output)
            NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                     (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                     (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                     (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                     (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
       }
    }
}
			
/**
 * La función devuelve un entero con el valor de los bits indicados.
 * Ejemplo:
 *		- valor de los pines: 0x0F0FAFF0
 *		- bit_inicial: 12 num_bits: 4
 *		- valor que retorna la función: 10 (lee los 4 bits 12-15)
 */
uint32_t hal_gpio_leer_n(uint32_t gpio_inicial, uint8_t num_bits) {
    uint32_t valor_pines = 0;
    uint32_t i;

    // Iterar sobre el número de bits que se deben leer
    for (i = 0; i < num_bits; i++) {
        uint32_t pin = gpio_inicial + i;

        // Leer el valor del pin actual y ajustar la posición en valor_pines
        if (NRF_GPIO->IN & (1 << pin)) {
            valor_pines |= (1 << i); // Si el pin está en alto (1), ajusta el bit correspondiente
        }
    }

    return valor_pines;
}


/**
 * Escribe en los bits indicados el valor 
 * (si valor no puede representarse en los bits indicados,
 *  se escribirá los num_bits menos significativos a partir del inicial).
 */
void hal_gpio_escribir_n(uint32_t gpio_inicial, uint8_t num_bits, uint32_t valor) {
    uint32_t i;

    for (i = 0; i < num_bits; i++) {
        uint32_t pin = gpio_inicial + i;

        // Verificar el valor del bit correspondiente en 'valor'
        if (valor & (1 << i)) {
            NRF_GPIO->OUTSET = (1 << pin); // Si el bit está en 1, pone el pin en alto
        } else {
            NRF_GPIO->OUTCLR = (1 << pin); // Si el bit está en 0, pone el pin en bajo
        }
    }
}

/* *****************************************************************************
 * Acceso a los GPIOs 
 *
 * a gpio unico (optimizar accesos)
 */

/**
 * El gpio se configuran como entrada o salida según la dirección.
 */
void hal_gpio_sentido(HAL_GPIO_PIN_T gpio, hal_gpio_pin_dir_t direccion){
        if (direccion == HAL_GPIO_PIN_DIR_INPUT) {
            // Configura el pin como entrada
           NRF_GPIO->PIN_CNF[gpio] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                     (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                     (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                     (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
        } else if (direccion == HAL_GPIO_PIN_DIR_OUTPUT) {
            // Configura el pin como salida
            NRF_GPIO->PIN_CNF[gpio] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                     (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                     (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                     (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                     (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
        }
}
 
/**
 * La función devuelve un entero (bool) con el valor de los bits indicados.
 */
uint32_t hal_gpio_leer(HAL_GPIO_PIN_T gpio){
	uint32_t x = (NRF_GPIO->IN >> gpio) & 0x01;
	return x ;// Desplazar y enmascarar para obtener el valor del pin específico
}


/**
 * Escribe en el gpio el valor
 */
void hal_gpio_escribir(HAL_GPIO_PIN_T gpio, uint32_t valor){
	  if (valor & 0x1) {
            NRF_GPIO->OUTSET = (1UL << gpio); // Si el bit está en 1, pone el pin en alto
        } else {
            NRF_GPIO->OUTCLR = (1UL << gpio); // Si el bit está en 0, pone el pin en bajo
        }
}

	



			


