#include "hal_consumo_nrf.h"
#include <nrf.h>

/* *****************************************************************************
 * Inicialización del hardware relacionado con el consumo
 * En esta plataforma, no es necesaria una inicialización específica.
 */
void hal_consumo_iniciar(void) {
    // No se requiere inicialización específica por ahora
}

/* *****************************************************************************
 * Poner el procesador en modo de espera utilizando WFI
 */
void hal_consumo_esperar(void) {
    // Instrucción CMSIS para poner el procesador en modo de espera hasta la próxima interrupción
    __WFI();
}

/* *****************************************************************************
 * Poner el procesador en modo de dormir (función pendiente de implementación)
 * Este modo aún no es necesario en la plataforma nRF, se implementará en el futuro.
 */
void hal_consumo_dormir(void) {
	NRF_POWER->SYSTEMOFF = POWER_SYSTEMOFF_SYSTEMOFF_Enter;
	while(1){
		__WFE();
	}
}
