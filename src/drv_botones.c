#include "drv_botones.h"
#include "board.h"       // Código específico para la configuración de pines
#include "rt_GE.h"       // Módulo gestor de eventos
#include "hal_tiempo.h"  // Módulo de alarmas para programar retardos
#include "hal_ext_int.h"    // Interfaz para manejo de pines GPIO

static uint32_t tiempo_ret = 100;
static boton_t botones[BUTTONS_NUMBER];
static uint32_t num_pulsados=0;

// Callback para manejo de eventos de pulsación
static void (*callback_encolar)(EVENTO_T ID_evento, uint32_t auxData);

#if BUTTONS_NUMBER > 0
	static const uint8_t boton_list[BUTTONS_NUMBER] = BUTTONS_LIST;
#endif

static void drv_cb_desde_hal(uint32_t pin){//pin to boton
	uint32_t botonPulsado;
		for (uint32_t i = 0; i < BUTTONS_NUMBER; i++){
			if (pin == boton_list[i]){
				botonPulsado = (i+1);
			}
		}
	callback_encolar(ev_PULSAR_BOTON, botonPulsado);
}

// Función de inicialización de los botones
uint32_t  drv_botones_iniciar(uint32_t EV_pulsar, uint32_t EV_retardo, void (*callback)(EVENTO_T ID_evento, uint32_t auxData)) {
	callback_encolar = callback;
	
	// Configuración de pines con la interfaz hal_gpio
	for (uint32_t i = 0; i<BUTTONS_NUMBER; i++){
		botones[i].pin = boton_list[i];
		botones[i].estado = E_REPOSO;	
		hal_gpio_sentido(boton_list[i], HAL_GPIO_PIN_DIR_INPUT);  
		hal_ext_int_iniciar(boton_list[i], (i+1), drv_cb_desde_hal);
		hal_ext_int_habilitar_despertar(boton_list[i]);         
	}
	svc_GE_suscribir(EV_pulsar, drv_botones_tratar);
	svc_GE_suscribir(EV_retardo, drv_botones_tratar);
	
	return BUTTONS_NUMBER;
}


// Función de tratamiento de eventos en la FSM
void drv_botones_tratar(uint32_t evento, uint32_t aux) {
		uint32_t botonPulsado;
		uint32_t pinPulsado;
	
		botonPulsado = aux;
		pinPulsado = boton_list[(aux - 1)];
		EstadoBoton_t estado_actual=botones[(botonPulsado-1)].estado;
    switch (estado_actual) {
        case E_REPOSO:
            svc_alarma_activar(tiempo_ret, ev_BOTON_RETARDO, botonPulsado);
            botones[botonPulsado-1].estado = E_ENTRANDO;
						num_pulsados++;
            break;

        case E_ENTRANDO:
            if (evento == ev_PULSAR_BOTON) {
            }
            else if (evento == ev_BOTON_RETARDO) {                
								uint32_t tiempo = tiempo_ret;
								tiempo |= 0x80000000;
								svc_alarma_activar(tiempo, ev_BOTON_RETARDO, botonPulsado); 
                botones[botonPulsado-1].estado = E_ESPERANDO;
            }
            break;

        case E_ESPERANDO:
            if (evento == ev_PULSAR_BOTON) {
            }
            else if (evento == ev_BOTON_RETARDO) {
								uint32_t estado_boton = hal_ext_int_leerINT(pinPulsado);
								//uint32_t estado_boton = hal_gpio_leer(pinPulsado);
								botones[botonPulsado-1].estado = E_SOLTADO;
								svc_alarma_activar(tiempo_ret, ev_BOTON_RETARDO, botonPulsado);                
										
									//} else {
										/*	if(num_pulsados > 1){
												svc_alarma_activar(1, ev_INACTIVIDAD, botonPulsado);      
											} else {
												svc_alarma_activar(1, ev_BOTON_RETARDO, botonPulsado); 
											}*/
											/*svc_alarma_activar(30, ev_BOTON_RETARDO, botonPulsado);                
											botones[botonPulsado-1].estado = E_SOLTADO;*/
									//}
            }
            break;

        case E_SOLTADO:
            if (evento == ev_PULSAR_BOTON) {
            }
            else if (evento == ev_BOTON_RETARDO) {
								hal_ext_int_habilitar_int(pinPulsado);

                botones[botonPulsado-1].estado = E_REPOSO;
								num_pulsados--;
            }
            break;

        default:
            estado_actual = E_REPOSO;
            break;
    }
}


