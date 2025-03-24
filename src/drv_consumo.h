/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los temporizadores
 * suministra los servicios independientemente del hardware
 */
 
#ifndef DRV_CONSUMO_H
#define DRV_CONSUMO_H

#include <stdint.h>

// Inicializaci�n del driver de consumo
void drv_consumo_iniciar(uint32_t monid);

// Poner el procesador en modo de espera (WFI)
void drv_consumo_esperar(void);

// Poner el procesador en modo de dormir (funci�n pendiente de implementaci�n futura)
void drv_consumo_dormir(void);

#endif // DRV_CONSUMO_H
