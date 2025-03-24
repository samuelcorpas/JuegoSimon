/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los temporizadores
 * suministra los servicios independientemente del hardware
 */
 
#ifndef DRV_CONSUMO_H
#define DRV_CONSUMO_H

#include <stdint.h>

// Inicialización del driver de consumo
void drv_consumo_iniciar(uint32_t monid);

// Poner el procesador en modo de espera (WFI)
void drv_consumo_esperar(void);

// Poner el procesador en modo de dormir (función pendiente de implementación futura)
void drv_consumo_dormir(void);

#endif // DRV_CONSUMO_H
