#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "rt_evento_t.h"
#include "drv_tiempo.h"
#include "rt_fifo.h"
#include "svc_alarma.h"
#include "drv_monitor.h"
#include "drv_consumo.h"
#include "ifdebug.h"


#define RT_GE_MAX_SUSCRITOS 4    // N�mero m�ximo de tareas por evento
//Funci�n que inicializa el gestor de eventos
void rt_GE_iniciar(uint32_t monitor);

//Funci�n que realiza toda la gesti�n de eventos 
void rt_GE_lanzador(void);

//Funci�n que subscribe un evento con su funci�n callback asociada
void svc_GE_suscribir(uint32_t evento, void (*f_callback)());

//Funci�n que cancela un evento subscrito
void svc_GE_cancelar(EVENTO_T evento, void (*f_callback)());

//Funci�n que actuar� de una forma u otra en cuesti�n del evento que se extrae
void rt_GE_tratar(EVENTO_T evento, uint32_t auxiliar);

#ifdef STATS
uint32_t eventos_encolados(uint32_t ID_evento);
#endif

#ifdef DEBUG
void test_GE_overflow(uint32_t id);
#endif
