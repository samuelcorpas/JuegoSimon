#ifndef RT_FIFO_H
#define RT_FIFO_H

#include "ifdebug.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "rt_evento_t.h"
#include "drv_tiempo.h"
#include "drv_monitor.h"


// Funci�n de inicializaci�n
void rt_FIFO_inicializar(uint32_t monitor_overflow);
// Funci�n para encolar un evento
void rt_FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData);
//Funcion de encolar eventos para el procesador modo usuario 
void rt_FIFO_encolar_USR(EVENTO_T ID_evento, uint32_t auxData);

// Funci�n para extraer un evento de la cola
uint8_t rt_FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData, Tiempo_us_t *TS);

#ifdef STATS
// Funci�n para obtener estad�sticas de un evento espec�fico
uint32_t rt_FIFO_estadisticas(uint32_t ID_evento);

uint32_t maximo_cola(void);
#endif

#ifdef DEBUG
void test_cola_overflow(uint32_t id);
void test_cola_desencola(uint32_t id);

#endif
#endif
