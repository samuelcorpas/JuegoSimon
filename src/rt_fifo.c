#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hal_concurrencia.h"
#include "rt_fifo.h"

#define FIFO_SIZE 64 // Tama�o de la cola circular

typedef struct {
    EVENTO_T id_evento;
    uint32_t auxData;
    Tiempo_us_t tiempo_us; // Timestamp en microsegundos
} EVENTO;

typedef struct {
    EVENTO buffer[FIFO_SIZE];  // Almacena los eventos
    uint32_t head;             // �ndice de la cabeza de la cola
    uint32_t tail;             // �ndice de la cola
    uint32_t count;            // N�mero de elementos en la cola
    uint32_t overflow_monitor; // Monitor de overflow (ID del monitor)
    uint32_t estadisticas[EVENT_TYPES]; // Estad�sticas de ocurrencia de eventos
} FIFO_T;


static FIFO_T fifo;  // Cola FIFO
#ifdef STATS
static uint32_t max_cola = 0;
#endif 

// Funci�n de inicializaci�n
void rt_FIFO_inicializar(uint32_t monitor_overflow) {
    fifo.head = 0;
    fifo.tail = 0;
    fifo.count = 0;
    fifo.overflow_monitor = monitor_overflow;
    #ifdef STATS
		for (size_t i = 0; i < 3; ++i) {
			fifo.estadisticas[i] = 0;  // Inicializa cada elemento a 0
		}
		#endif
}



// Funci�n para encolar un evento
void rt_FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {
    // Verifica si la cola est� llena
    if (fifo.count >= FIFO_SIZE) {
			 drv_monitor_marcar(fifo.overflow_monitor);
        while (1) {
        }
		}
    fifo.count++;
    // A�ade el evento a la cola y avanza el �ndice head
    fifo.buffer[fifo.head].id_evento = ID_evento;
    fifo.buffer[fifo.head].auxData = auxData;
    fifo.buffer[fifo.head].tiempo_us = drv_tiempo_actual_ms(); // Agrega timestamp
    
    fifo.head = (fifo.head + 1) % FIFO_SIZE;
    #ifdef STATS
		if (fifo.count > max_cola){
			max_cola = fifo.count;
		}
    // Actualiza estad�sticas
    fifo.estadisticas[ID_evento]++;
    if (ID_evento != 0){
			fifo.estadisticas[0]++;
		}
		#endif 

}

void rt_FIFO_encolar_USR(EVENTO_T ID_evento, uint32_t auxData) {
	  SC_entrar_disable_irq();
		rt_FIFO_encolar(ID_evento, auxData);
		SC_salir_enable_irq();
}

uint8_t rt_FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData, Tiempo_us_t *TS) {
    SC_entrar_disable_irq();
		if (fifo.count == 0) {
				SC_salir_enable_irq();
        return 0;  // La cola est� vac�a
    }

    // Extrae el evento m�s antiguo
    *ID_evento = fifo.buffer[fifo.tail].id_evento;
    *auxData = fifo.buffer[fifo.tail].auxData;
    *TS = fifo.buffer[fifo.tail].tiempo_us;
    
    // Avanza el �ndice tail y reduce el conteo de la cola
    fifo.tail = (fifo.tail + 1) % FIFO_SIZE;
    fifo.count--;
		
		SC_salir_enable_irq();

    return fifo.count + 1;  // Retorna el n�mero de eventos sin procesar
}

#ifdef STATS
// Funci�n para obtener estad�sticas de un evento espec�fico
uint32_t rt_FIFO_estadisticas(uint32_t ID_evento) {
        return fifo.estadisticas[ID_evento];  // Devuelve el conteo de ese evento
}

uint32_t maximo_cola(void){
	return max_cola;
}
#endif

#ifdef DEBUG
void test_cola_overflow(uint32_t id){
	rt_FIFO_inicializar(4);
	for (int i = 0; i<64; i++){
		rt_FIFO_encolar(ev_T_PERIODICO, i);
	}	
	
}

void test_cola_desencola(uint32_t id){
	rt_FIFO_inicializar(4);
	EVENTO_T evento;
	uint32_t dato;
	Tiempo_us_t tiempo;
	for (int i = 0; i<10; i++){
		rt_FIFO_encolar(ev_T_PERIODICO, i);
	}	
	uint32_t datos0 = fifo.estadisticas[0];
	uint32_t datos1 = fifo.estadisticas[1];

	for (int i = 0; i<10; i++){
		rt_FIFO_extraer(& evento, & dato, & tiempo);
	}	
	
}

void eventos_encolados(void){
	for (uint32_t i=0; i<EVENT_TYPES; i++){
		
	}
}
#endif

