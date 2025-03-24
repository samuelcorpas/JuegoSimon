#include "drv_consumo.h"
#include "hal_consumo.h"
#include "drv_monitor.h"

/* *****************************************************************************
 * Inicialización del driver de consumo
 * Por ahora, no requiere inicialización específica, pero mantenemos la función
 * por consistencia con los demás módulos.
 */
void drv_consumo_iniciar(uint32_t monid) {
    // Llamamos a la función de inicialización del HAL si fuese necesario
    hal_consumo_iniciar();
		drv_monitor_iniciar();
}

/* *****************************************************************************
 * Poner el procesador en modo de espera (WFI)
 */
void drv_consumo_esperar(void) {
    // Llama a la función del HAL para esperar
		drv_monitor_desmarcar(3);
    hal_consumo_esperar();
		drv_monitor_marcar(3);
}

/* *****************************************************************************
 * Poner el procesador en modo de dormir (futuro)
 * Este modo aún no está implementado en la plataforma nRF, pero
 * dejamos la estructura preparada.
 */
void drv_consumo_dormir(void) {
    // Llamará a una función del HAL cuando se implemente
    hal_consumo_dormir();
}
