#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hal_WDT.h"
#include "rt_GE.h"

#ifdef STATS
static uint32_t max_eventos_subs[EVENT_TYPES] = {};
	Tiempo_ms_t tiempo_en_espera;
#endif

// Estructura de la tarea suscrita a un evento
typedef struct {
    void (*f_callback)(uint32_t, uint32_t);  // Función de callback
    uint32_t auxData;  // Datos auxiliares de la tarea suscrita
} rt_GE_tarea_t;

// Estructura del gestor de eventos
typedef struct {
    rt_GE_tarea_t tareas_suscritas[EVENT_TYPES][RT_GE_MAX_SUSCRITOS]; // Tareas suscritas por tipo de evento
    uint32_t num_tareas_suscritas[EVENT_TYPES];  // Número de tareas suscritas por evento
} rt_GE_t;

static rt_GE_t gestor_eventos;  // Instancia del gestor de eventos
static uint32_t mon_overflow;



void rt_GE_iniciar(uint32_t monitor){
	//rt_FIFO_inicializar(3);
	for (int i = 0; i < EVENT_TYPES; ++i) {
        gestor_eventos.num_tareas_suscritas[i] = 0;  // Ninguna tarea suscrita inicialmente
        for (int j = 0; j < RT_GE_MAX_SUSCRITOS; ++j) {
            gestor_eventos.tareas_suscritas[i][j].f_callback = NULL;  // No hay callback asignado inicialmente
            gestor_eventos.tareas_suscritas[i][j].auxData = 0;  // No hay datos auxiliares
        }
   }
	svc_GE_suscribir(ev_INACTIVIDAD, rt_GE_tratar);
	svc_GE_suscribir(ev_PULSAR_BOTON, rt_GE_tratar);

}

void rt_GE_lanzador(){
	EVENTO_T evento;
  uint32_t auxData;
  Tiempo_us_t timestamp;
    
		svc_alarma_activar(200000, ev_INACTIVIDAD, 0);  // 10 segundos como ejemplo de timeout
    // Intentamos extraer un evento de la cola FIFO
		while(1){
			hal_WDT_feed();
			if (rt_FIFO_extraer(&evento, &auxData, &timestamp) != 0) {
					// Si hay un evento, lo despachamos a las tareas suscritas
							// Llamamos al callback de todas las tareas suscritas a este evento
							for (int i = 0; i < gestor_eventos.num_tareas_suscritas[evento]; ++i) {
									if (gestor_eventos.tareas_suscritas[evento][i].f_callback != NULL) {
											// Llamamos al callback con el evento y los datos auxiliares
											gestor_eventos.tareas_suscritas[evento][i].f_callback(evento, auxData);
									}
							}
			} else {
					// Si no hay eventos, ponemos el sistema en espera
					#ifdef STATS
						Tiempo_ms_t tiempo1 = drv_tiempo_actual_ms();
					#endif
					drv_consumo_esperar();
					#ifdef STATS
						Tiempo_ms_t tiempo2 = drv_tiempo_actual_ms();
						tiempo_en_espera = tiempo_en_espera + (tiempo2 - tiempo1);
					#endif
			}
		}
}

void svc_GE_suscribir(uint32_t evento, void (*f_callback)()){

	if (gestor_eventos.num_tareas_suscritas[evento] < RT_GE_MAX_SUSCRITOS) {
        int idx = gestor_eventos.num_tareas_suscritas[evento];
        gestor_eventos.tareas_suscritas[evento][idx].f_callback = f_callback;
        gestor_eventos.tareas_suscritas[evento][idx].auxData = 0;
        gestor_eventos.num_tareas_suscritas[evento]++;
				#ifdef STATS
				  max_eventos_subs[evento]++;
				#endif
    } else {
        // Si no hay espacio para más tareas suscritas, gestionamos el overflow
        while (1) {
						drv_monitor_marcar(mon_overflow);
        }
    }
}

void svc_GE_cancelar(EVENTO_T evento, void (*f_callback)()){
	bool encontrado = false;
    for (int i = 0; i < gestor_eventos.num_tareas_suscritas[evento]; ++i) {
        if (gestor_eventos.tareas_suscritas[evento][i].f_callback == f_callback) {
            for (int j = i; j < gestor_eventos.num_tareas_suscritas[evento] - 1; ++j) {
                gestor_eventos.tareas_suscritas[evento][j] = gestor_eventos.tareas_suscritas[evento][j + 1];
            }
            gestor_eventos.num_tareas_suscritas[evento]--;
            encontrado = true;
            break;
        }
    }
    if (!encontrado) {
        while (1) {
            drv_monitor_marcar(mon_overflow);
        }
    }
	}
void rt_GE_tratar(EVENTO_T evento, uint32_t auxiliar){
	if (evento == ev_PULSAR_BOTON) {
        // Si el evento es de usuario, reprogramamos la alarma de inactividad
        svc_alarma_activar(20000, ev_INACTIVIDAD, 0);  // 20 segundos de tiempo de espera
    } else if (evento == ev_INACTIVIDAD) {
        // Si la alarma de inactividad vence, ponemos el sistema en modo de bajo consumo
        drv_consumo_dormir();
    }
}

#ifdef STATS
uint32_t eventos_encolados(uint32_t ID_evento) {
        return max_eventos_subs[ID_evento];  // Devuelve el conteo de ese evento
}
#endif

#ifdef DEBUG
void test_GE_overflow(uint32_t id){
	rt_GE_iniciar(id);
	for (int i = 0; i<(RT_GE_MAX_SUSCRITOS+1); i++){
		svc_GE_suscribir(ev_T_PERIODICO, rt_FIFO_encolar);
	}	
	
}
#endif

