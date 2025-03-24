#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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
#include "random.h"
#include "simon.h"
typedef enum {
	e_INIT,
	e_SHOW_SEQUENCE,
	e_WAIT_FOR_INPUT,
	e_SUCCESS,
	e_FAIL
} _GameState;

static uint32_t vector_leds[8] = {}; // Declaración e inicialización
static uint32_t indice;
static uint32_t indice_pulsado;
static uint32_t indice_led;
static _GameState Estado_Simon = e_INIT;
static uint32_t iter;
static uint32_t leds_encendidos = 0;
	
static uint32_t tiempo_sin_pulsar = 5000;
static uint32_t tiempo_espera_led = 800;
static uint32_t tiempo_espera_inicial = 800;
static uint32_t tiempo_simon = 100;
static uint32_t tiempo_espera_encender = 800;

static uint32_t Num_leds; 
static uint32_t Num_botones; 
	
#ifdef STATS
	Tiempo_ms_t tiempo_pulsacion;
	Tiempo_ms_t tiempo2;
	Tiempo_ms_t tiempo1;
#endif


void simon_iniciar(uint32_t botones, uint32_t leds){
	Num_leds = leds;
	Num_botones = botones;
	random_init(123456789); // Usamos una semilla fija para obtener la misma secuencia cada vez
	uint32_t valor;
	for (uint32_t i = 0; i < 8; i++){
		valor = random_generate(1, Num_botones);
		vector_leds[i] = valor;
	}
	svc_GE_suscribir(ev_PULSAR_BOTON, simon);
	svc_GE_suscribir(ev_SIMON, simon);
	svc_GE_suscribir(ev_ALARMA_BOTON, simon);
	svc_GE_suscribir(ev_PARPADEO, simon);
	svc_GE_suscribir(ev_ESPERA_LED, simon);
	svc_alarma_activar(tiempo_simon, ev_SIMON, 5);
	rt_GE_lanzador();
}
	
//Esta funcion contiene el desarrollo del juego Simon
	
void simon(uint32_t evento, uint32_t boton){
	if(indice < 8){
		switch (Estado_Simon) {
        case e_INIT:
				 if(evento == ev_SIMON){
						indice_pulsado=0;
						for (uint32_t i=0; i<=Num_leds; i++){
							drv_led_encender(i);
						}
						svc_alarma_activar(tiempo_espera_led, ev_PARPADEO, 6);  //Activamos alarma de parpadeo de secuencia incial
					}
					else if (evento == ev_PARPADEO){
						for (uint32_t i=0; i<=Num_leds; i++){
							drv_led_apagar(i);
						}
						Estado_Simon = e_SHOW_SEQUENCE; 
						svc_alarma_activar(tiempo_simon, ev_SIMON, 5);
					}
				break;
					
        case e_SHOW_SEQUENCE:
					if(evento == ev_SIMON){	
						iter = (indice + 1);
						leds_encendidos = 0;
						if (tiempo_espera_led > 200){
							tiempo_espera_led = tiempo_espera_led - 200;
							tiempo_espera_encender = tiempo_espera_encender - 200;
						} else {
							tiempo_espera_led = 100;
							tiempo_espera_encender = 100;						
						}
						svc_alarma_activar(tiempo_espera_led, ev_PARPADEO, 6);
					} else if (evento == ev_PARPADEO){
						drv_led_encender(vector_leds[indice_led]);
						svc_alarma_activar(tiempo_espera_led, ev_ESPERA_LED, 7);
					} else if (evento == ev_ESPERA_LED){
						drv_led_apagar(vector_leds[indice_led]);
						indice_led++;
						leds_encendidos++;
						if (leds_encendidos < iter){
							svc_alarma_activar(tiempo_espera_led, ev_PARPADEO, 6);
						} else {
								#ifdef STATS
								tiempo1 = drv_tiempo_actual_ms();
								#endif
								
								indice++;
								Estado_Simon = e_WAIT_FOR_INPUT; 
								svc_alarma_activar(tiempo_sin_pulsar, ev_ALARMA_BOTON, 0);
						}
					}
				break;

        case e_WAIT_FOR_INPUT:
					#ifdef STATS
						tiempo2 = drv_tiempo_actual_ms();
						tiempo_pulsacion = tiempo2 - tiempo1;
					#endif
				
					if(boton == vector_leds[indice_pulsado]){
						indice_pulsado++;
						if(indice_pulsado == indice_led){
							Estado_Simon = e_SUCCESS;
							svc_alarma_activar(tiempo_simon, ev_SIMON, 0);	
						}else{
							svc_alarma_activar(tiempo_sin_pulsar, ev_ALARMA_BOTON, 0);							
						}
					}else{
						Estado_Simon = e_FAIL;
						svc_alarma_activar(tiempo_simon, ev_SIMON, 5);
					}
        break;

        case e_SUCCESS:
					if(evento == ev_SIMON){
						svc_alarma_activar(0, ev_ALARMA_BOTON, 5);
						for (uint32_t i=0; i<=Num_leds; i++){
							drv_led_encender(i);
						}
						svc_alarma_activar(tiempo_espera_inicial, ev_PARPADEO, 6);  //Activamos alarma de parpadeo de secuencia incial
					}
					else if (evento == ev_PARPADEO){
						for (uint32_t i=0; i<=Num_leds; i++){
							drv_led_apagar(i);
						}
						indice_pulsado = 0;
						indice_led = 0;
						Estado_Simon = e_SHOW_SEQUENCE; 
						svc_alarma_activar(tiempo_simon, ev_SIMON, 5);
					}
				break;
				
				case e_FAIL:
					if (evento == ev_SIMON){
						leds_encendidos=0;
						svc_alarma_activar(0, ev_ALARMA_BOTON, 5);
						svc_alarma_activar(tiempo_simon, ev_PARPADEO, 6);
					} else if(evento == ev_PARPADEO){
						if (leds_encendidos<Num_botones){
							drv_led_encender(leds_encendidos+1);
						}
						svc_alarma_activar(tiempo_espera_inicial, ev_ESPERA_LED, 7);
					} else if(evento == ev_ESPERA_LED){
						drv_led_apagar(leds_encendidos+1);
						leds_encendidos++;
						if (leds_encendidos<Num_botones){
							svc_alarma_activar(tiempo_simon, ev_PARPADEO, 6);
						}	else {
							indice_pulsado = 0;
							indice_led = 0;
							indice = 0;
							tiempo_espera_led = tiempo_espera_inicial;
							tiempo_espera_encender = tiempo_espera_inicial;
							Estado_Simon = e_SHOW_SEQUENCE;
							svc_alarma_activar(tiempo_simon, ev_SIMON, 5);
						}
					}
        break;

        default:
				break;
			}
		}
}
