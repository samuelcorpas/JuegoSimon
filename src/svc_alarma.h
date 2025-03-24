#ifndef SVC_ALARMA_H  // Nombre único para evitar colisiones
#define SVC_ALARMA_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "rt_evento_t.h"


//Iniciamos toda la estructura de las alarmas
void svc_alarma_iniciar(uint32_t overflow, void (*f_callback_USR)(), uint32_t ID_evento, void (*f_callback)());

//Activamos una alarma para dentro de retardo_ms ms con el evento ID_evento
void svc_alarma_activar(uint32_t retardo_ms, uint32_t ID_evento, uint32_t auxData);

//Funcion que actua de una forma u otra cuando le llega una alarma
void svc_alarma_tratar(EVENTO_T evento, uint32_t aux);

#endif  // Cierra la guarda de inclusión

#ifdef STATS
uint32_t maximo_alarmas_activas(void);	

uint32_t totales_alarmas(void);
#endif
	
#ifdef DEBUG
void test_alarma_overflow(uint32_t id);
#endif
