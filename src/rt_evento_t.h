#ifndef RT_EVENTO_T_H
#define RT_EVENTO_T_H


#include <stdint.h>
typedef enum {
	ev_VOID = 0, // default, vacio
	ev_T_PERIODICO = 1, //
	ev_PULSAR_BOTON = 2,
	ev_INACTIVIDAD = 3,
	ev_BOTON_RETARDO = 4,
	ev_ALARMA_BOTON = 5,
	ev_SIMON = 6,
	ev_PARPADEO = 7,
	ev_ESPERA_LED = 8
} EVENTO_T; //mapea a uint32_t

#define EVENT_TYPES 9
#define ev_NUM_EV_USUARIO 1
#define ev_USUARIO {ev_PULSAR_BOTON}

#endif 

