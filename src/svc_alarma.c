
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "svc_alarma.h"
#include "rt_GE.h"
#include "drv_tiempo.h"
#include "drv_monitor.h"
#include "ifdebug.h"

#define PERIODO_ALARMA_MS 100
#define MASK_PERIODIC_BIT 0x80000000  // Bit de mayor peso para definir periodicidad
#define MASK_DELAY 0x7FFFFFFF         // Mascara para el retardo en ms

static void(*fun_callback)();					//callback que sera utilizado por interrupciones
static void(*fun_callback_USR)();			//callback que sera utilizado en modo usuario 
static uint32_t mon_overflow;

#ifdef STATS
static uint32_t num_activas_max = 0;
static uint32_t num_activas = 0;
static uint32_t num_totales = 0;
#endif 

// Configuración de la cola y definición de tipos
#define svc_ALARMAS_MAX 4

typedef struct {
    uint32_t retardo_ms; // Tiempo en ms; el bit más significativo indica periodicidad
		uint32_t tiempo_inicial; // Almacenamos el valor original del retardo
    EVENTO_T ID_evento;  // Identificador del evento
    uint32_t auxData;    // Datos auxiliares a pasar al evento
    bool activa;         // Estado de la alarma (activa/inactiva)
		bool periodica;
} ALARMA_T;

static ALARMA_T alarmas[svc_ALARMAS_MAX];

//static int num_alarmas_activas = 0; // Contador de alarmas activas

void svc_alarma_iniciar(uint32_t overflow, void (*f_callback_USR)(), uint32_t ID_evento, void (*f_callback)()){
		fun_callback = f_callback;										//se usara de callback la función encolar, Sera usada por las interrupciones
		fun_callback_USR = f_callback_USR;						//en caso de que la alarma se active desde modo usuario, como las interrupciones estan activas, se desactivan en seccion critica
		mon_overflow = overflow;
		svc_GE_suscribir(ID_evento, svc_alarma_tratar);
		//num_alarmas_activas = 0;
		for (int i = 0; i < svc_ALARMAS_MAX; i++) {
				alarmas[i].activa = false;
		}
		drv_tiempo_periodico_ms(PERIODO_ALARMA_MS, fun_callback, ID_evento);

}
void svc_alarma_activar(uint32_t retardo_ms, uint32_t ID_evento, uint32_t auxData){
		
	bool es_periodica = (retardo_ms & MASK_PERIODIC_BIT) != 0;  // Verificar bit de periodicidad
  uint32_t retardo_mask = retardo_ms & MASK_DELAY;                 // Extraer el retardo en ms
	uint32_t retardo = retardo_mask / 100;
    // Si el retardo es 0, cancelar alarma
    if (retardo == 0) {
        for (int i = 0; i < svc_ALARMAS_MAX; i++) {
            if (alarmas[i].activa && alarmas[i].ID_evento == ID_evento) {
                alarmas[i].activa = false;  // Cancelar la alarma
								#ifdef STATS
									num_activas--;
								#endif
                return;  //Hacemos return porque ya hemos encontrado la alarma y hemos hecho el objetivo que teniamos
            }
        }
        return;  // Hacemos return porque quieres cancelar una alarma que no existe
    }
		
		//En caso de que la alarma para ese evento ya exista la reprogramamos con el tiempo inciial
		for (int i = 0; i < svc_ALARMAS_MAX; i++) {
        if (alarmas[i].activa && alarmas[i].ID_evento == ID_evento) {
            // Reprogramar alarma existente
            alarmas[i].retardo_ms = retardo;
            alarmas[i].periodica = es_periodica;
            alarmas[i].auxData = auxData;
						alarmas[i].tiempo_inicial = retardo;
						#ifdef STATS
							num_totales++;
						#endif
            return;		//Hacemos return porque se ha encontrado la alarma que se queria activar y se ha reprogramado
        }
    }

    // En caso de que la alarma sea nueva, se busca el hueco y se añade 
    for (int i = 0; i < svc_ALARMAS_MAX; i++) {
        if (!alarmas[i].activa) {
            alarmas[i].activa = true;
            alarmas[i].periodica = es_periodica;
            alarmas[i].retardo_ms = retardo;
            alarmas[i].ID_evento = ID_evento;
            alarmas[i].auxData = auxData;
						alarmas[i].tiempo_inicial = retardo;
						#ifdef STATS
							num_activas++;
							num_totales++;
							if (num_activas_max < num_activas){
								num_activas_max = num_activas;
							}
						#endif
            return;
        }
    }

    // Si no hay alarmas disponibles, indicar desbordamiento (overflow)
    while (1) {
			drv_monitor_marcar(mon_overflow);
    }
	}


void svc_alarma_tratar(EVENTO_T evento, uint32_t aux){
  EVENTO_T id_evento;
	uint32_t auxData;
	if (evento == ev_T_PERIODICO){
		for (int i = 0; i < svc_ALARMAS_MAX; i++) {
					if (alarmas[i].activa) {
							if (alarmas[i].retardo_ms > 0) {
									alarmas[i].retardo_ms--;
							}

							if (alarmas[i].retardo_ms == 0) {
									id_evento = alarmas[i].ID_evento;
									auxData = alarmas[i].auxData;
									// LLamar callback asociado a la alarma 
									fun_callback_USR(id_evento, auxData);
									
									// Verificar si la alarma es periódica
									if (alarmas[i].periodica) {
											alarmas[i].retardo_ms = alarmas[i].tiempo_inicial;
									} else {
											alarmas[i].activa = false;
									}
							}
					}
			}
		}
}

#ifdef STATS
uint32_t maximo_alarmas_activas(void){
	return num_activas_max;
}	

uint32_t totales_alarmas(void){
	return num_totales;
}	
#endif

#ifdef DEBUG
void test_alarma_overflow(uint32_t id){
	svc_alarma_iniciar(4, rt_FIFO_encolar, ev_T_PERIODICO, rt_FIFO_encolar_USR);
		svc_alarma_activar(1000, ev_T_PERIODICO, 1);
		svc_alarma_activar(1000, ev_BOTON_RETARDO, 1);
		svc_alarma_activar(1000, ev_INACTIVIDAD, 1);
		svc_alarma_activar(1000, ev_VOID, 1);
		svc_alarma_activar(1000, ev_PULSAR_BOTON, 1);
		
}
#endif

