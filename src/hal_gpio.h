/* *****************************************************************************
 * P.H.2024: hal_gpio, interface que nos independiza del hardware concreto
 */
 
#ifndef HAL_GPIO
#define HAL_GPIO

#include <stdint.h>

/**
 * Dirección de los registros de GPIO (E o S)
 */
 
enum {
	HAL_GPIO_PIN_DIR_INPUT = 0, 
	HAL_GPIO_PIN_DIR_OUTPUT = 1,
} typedef hal_gpio_pin_dir_t;

/**
 * Tipo de datos para los pines
 */
typedef uint32_t HAL_GPIO_PIN_T; 


/**
 * Permite emplear el GPIO y debe ser invocada antes
 * de poder llamar al resto de funciones de la biblioteca.
 * re-configura todos los pines como de entrada (para evitar cortocircuitos)
 */
void hal_gpio_iniciar(void);


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
void hal_gpio_sentido_n(HAL_GPIO_PIN_T gpio_inicial, 
			uint8_t num_bits, hal_gpio_pin_dir_t direccion);
 
/**
 * La función devuelve un entero con el valor de los bits indicados.
 * Ejemplo:
 *		- valor de los pines: 0x0F0FAFF0
 *		- bit_inicial: 12 num_bits: 4
 *		- valor que retorna la función: 10 (lee los 4 bits 12-15)
 */
uint32_t hal_gpio_leer_n(HAL_GPIO_PIN_T gpio_inicial, uint8_t num_bits);


/**
 * Escribe en los bits indicados el valor 
 * (si valor no puede representarse en los bits indicados,
 *  se escribirá los num_bits menos significativos a partir del inicial).
 */
void hal_gpio_escribir_n(HAL_GPIO_PIN_T bit_inicial, 
			uint8_t num_bits, uint32_t valor);

/* *****************************************************************************
 * Acceso a los GPIOs 
 *
 * a gpio unico (optimizar accesos a un solo bit)
 */

/**
 * El gpio se configuran como entrada o salida según la dirección.
 */
void hal_gpio_sentido(HAL_GPIO_PIN_T gpio, hal_gpio_pin_dir_t direccion);
 
/**
 * La función devuelve un entero (bool), sera cero si el gpio es cero, sera distinto de cero en caso contrario.
 */
uint32_t hal_gpio_leer(HAL_GPIO_PIN_T gpio);


/**
 * Escribe en el gpio el valor
 */
void hal_gpio_escribir(HAL_GPIO_PIN_T gpio, uint32_t valor);

#endif
