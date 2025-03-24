/* *****************************************************************************
  * P.H.2024: Driver/Manejador de los Leds
	*
	* blink practica 2 de proyecto hardware 2024
 */
 
#include <stdint.h>
#include <stdbool.h>
#include "ifdebug.h"
#include "hal_gpio.h"
#include "drv_leds.h"
#include "drv_tiempo.h"
#include "rt_GE.h"
#include "rt_evento_t.h"
#include "drv_consumo.h"
#include "drv_botones.h"
#include "hal_WDT.h"
#include "rt_fifo.h"
#include "simon.h"

static uint32_t Num_leds; 
static uint32_t Num_botones; 
	
#ifdef STATS
	Tiempo_ms_t tiempo_pulsacion;
	Tiempo_ms_t tiempo2;
	Tiempo_ms_t tiempo1;
#endif

#ifdef DEBUG
void pruebaWDT(uint32_t id){
			drv_led_encender(4);

			hal_WDT_iniciar(10);

			drv_led_encender(id);
			uint32_t i = 0;
			while (1) {
					// El WDT no se alimentará, por lo tanto, el sistema se reiniciará
			i++;

			if(i == 100){
				hal_WDT_feed();
			}
			}


			}
#endif

/* *****************************************************************************
 * MAIN, Programa principal.
 * para la primera sesion se debe usar la funcion de blink_v1 sin temporizadores
 * para la entrega final se debe incocar a blink_v2
 */
int main(void){
	//uint32_t Num_Leds, Num_Botones;

	hal_gpio_iniciar();	// llamamos a iniciar gpio antesde que lo hagan los drivers
	
	/* Configure LED */
	Num_leds = drv_leds_iniciar();
	drv_tiempo_iniciar();
	rt_FIFO_inicializar(4);
	rt_GE_iniciar(4);
	svc_alarma_iniciar(4, rt_FIFO_encolar_USR, ev_T_PERIODICO, rt_FIFO_encolar);
	hal_WDT_iniciar(6);
	Num_botones = drv_botones_iniciar(ev_PULSAR_BOTON, ev_BOTON_RETARDO, rt_FIFO_encolar);
	if (Num_leds > 0){
		
		#ifdef DEBUG
			drv_led_encender(1);
			test_GE_overflow(4);
			drv_led_encender(2);
			test_cola_desencola(4);
			test_cola_overflow(4);
			pruebaWDT(3);
			test_alarma_overflow(4);

		#endif
		simon_iniciar(Num_botones, Num_leds);
		
		
			#ifdef STATS
			uint32_t veces_encolado[EVENT_TYPES] = {};
			for (uint32_t i=0; i<EVENT_TYPES; i++){
				veces_encolado[i] = rt_FIFO_estadisticas(i);
			}
			uint32_t max_cola = maximo_cola();
			uint32_t alarmas_actitivas = maximo_alarmas_activas();
			uint32_t alarmas_totales = totales_alarmas();
			uint32_t eventos_suscritos[EVENT_TYPES] = {};
			for (uint32_t i=0; i<EVENT_TYPES; i++){
				eventos_suscritos[i] = eventos_encolados(i);
			}
			#endif

	}
}
