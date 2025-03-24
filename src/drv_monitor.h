/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los Monitores
 * suministra los servicios de iniciar, encender, apagar, ... independientemente del hardware
 */

#ifndef DRV_MONITOR
#define DRV_MONITOR

#include <stdint.h>

/**
 * inicializa los monitores, los deja desmarcados y devuelve el numero de monitores disponibles en la plataforma
 */
uint32_t drv_monitor_iniciar(void);

/**
 * marca el monitor
 */
void drv_monitor_marcar(uint32_t id);

/**
 * desmarca el monitor
 */
void drv_monitor_desmarcar(uint32_t id);


#endif
