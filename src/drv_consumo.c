#include "drv_consumo.h"
#include "hal_consumo.h"
#include "drv_monitor.h"

/* *****************************************************************************
 * Inicializaci�n del driver de consumo
 * Por ahora, no requiere inicializaci�n espec�fica, pero mantenemos la funci�n
 * por consistencia con los dem�s m�dulos.
 */
void drv_consumo_iniciar(uint32_t monid) {
    // Llamamos a la funci�n de inicializaci�n del HAL si fuese necesario
    hal_consumo_iniciar();
		drv_monitor_iniciar();
}

/* *****************************************************************************
 * Poner el procesador en modo de espera (WFI)
 */
void drv_consumo_esperar(void) {
    // Llama a la funci�n del HAL para esperar
		drv_monitor_desmarcar(3);
    hal_consumo_esperar();
		drv_monitor_marcar(3);
}

/* *****************************************************************************
 * Poner el procesador en modo de dormir (futuro)
 * Este modo a�n no est� implementado en la plataforma nRF, pero
 * dejamos la estructura preparada.
 */
void drv_consumo_dormir(void) {
    // Llamar� a una funci�n del HAL cuando se implemente
    hal_consumo_dormir();
}
