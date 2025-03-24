#ifndef DRV_BOTONES_H
#define DRV_BOTONES_H

#include <stdint.h>
#include "hal_gpio.h"    // Interfaz para manejo de pines GPIO
#include "rt_evento_t.h"

// Definici�n de los estados de la FSM
typedef enum {
    E_REPOSO,
    E_ENTRANDO,
    E_ESPERANDO,
    E_SOLTADO
} EstadoBoton_t;

typedef struct {
    HAL_GPIO_PIN_T pin;                     // Pin del bot�n
    EstadoBoton_t estado;                  // Estado actual del bot�n
    uint32_t ultimo_tiempo;                 // �ltimo tiempo de cambio de estado
   // void (*callbackStruct)(EVENTO_T evento, uint32_t aux);                 // Funci�n callback a ejecutar
} boton_t;

// Inicializaci�n de los botones
uint32_t drv_botones_iniciar(uint32_t evento1, uint32_t evento2, void (*callback)(EVENTO_T ID_evento, uint32_t auxData));

// Tratamiento de eventos de los botones
void drv_botones_tratar(uint32_t evento, uint32_t aux);

#endif // DRV_BOTONES_H
