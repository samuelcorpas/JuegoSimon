#include "hal_consumo_nrf.h"
#include <nrf.h>

/* *****************************************************************************
 * Inicializaci�n del hardware relacionado con el consumo
 * En esta plataforma, no es necesaria una inicializaci�n espec�fica.
 */
void hal_consumo_iniciar(void) {
    // No se requiere inicializaci�n espec�fica por ahora
}

/* *****************************************************************************
 * Poner el procesador en modo de espera utilizando WFI
 */
void hal_consumo_esperar(void) {
    // Instrucci�n CMSIS para poner el procesador en modo de espera hasta la pr�xima interrupci�n
    __WFI();
}

/* *****************************************************************************
 * Poner el procesador en modo de dormir (funci�n pendiente de implementaci�n)
 * Este modo a�n no es necesario en la plataforma nRF, se implementar� en el futuro.
 */
void hal_consumo_dormir(void) {
	NRF_POWER->SYSTEMOFF = POWER_SYSTEMOFF_SYSTEMOFF_Enter;
	while(1){
		__WFE();
	}
}
