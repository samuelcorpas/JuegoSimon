#include "hal_ext_int.h"
#include <nrf.h>              // Funciones específicas del microcontrolador nRF
#include "board.h"

//static void (*ext_int_callback)(EVENTO_T ID_evento, uint32_t auxData);  // Puntero a la función de callback
//static EVENTO_T Id_evento_cb;
//static uint32_t pin_cb;
static uint32_t channel = 0; // Canal GPIOTE (0-7)

void (*callbacks[BUTTONS_NUMBER])(uint32_t auxData) = {};


typedef struct {
    uint32_t auxData;
		//uint32_t pin;
		uint32_t canal_pin;
    void (*callback)(uint32_t auxData);
		//void (*callback)(uint32_t pin);
} BotonCallbackInfo;

BotonCallbackInfo boton_callbacks[BUTTONS_NUMBER]; // Información de callbacks por botón

// Función para inicializar los botones con GPIOTE
void hal_ext_int_iniciar(uint32_t pin, uint32_t auxData, void (*callback)(uint32_t auxData)) {
    //static uint32_t channel = 0; // Canal GPIOTE (0-7)
    if (channel >= 8) {
        // No hay canales disponibles, manejar error aquí
        return;
    }
		NRF_GPIOTE->CONFIG[channel] = 0;
		NRF_GPIOTE->INTENCLR = (1 << channel);

    // Configurar el canal GPIOTE para detectar cambios en el pin
    NRF_GPIOTE->CONFIG[channel] = (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos) |
                                  (pin << GPIOTE_CONFIG_PSEL_Pos) |
                                  (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos);

    // Habilitar la interrupción del canal GPIOT

    // Almacenar los datos del callback
    boton_callbacks[channel].auxData = pin;
    //boton_callbacks[channel].pin = pin;
		boton_callbacks[channel].canal_pin = channel;
		//habria que pasar el pin del boton que estamos pulsandi
    boton_callbacks[channel].callback = callback;
		hal_ext_int_habilitar_int(pin);

    // Incrementar el canal para el siguiente botón
    channel++;
}

// Manejador de interrupciones de GPIOTE
void GPIOTE_IRQHandler(void) {
    for (uint32_t channel = 0; channel < BUTTONS_NUMBER; channel++) {
        if (NRF_GPIOTE->EVENTS_IN[channel]) {
            NRF_GPIOTE->EVENTS_IN[channel] = 0; // Limpiar el evento

            // Llamar al callback asociado, si está definido
            if (boton_callbacks[channel].callback) {
                boton_callbacks[channel].callback(boton_callbacks[channel].auxData);
								//boton_callbacks[channel].callback(boton_callbacks[channel].pin);
								hal_ext_int_deshabilitar_int(boton_callbacks[channel].auxData);
								//hal_ext_int_deshabilitar_int(boton_callbacks[channel].pin);
            }
        }
    }
}

// Función para habilitar la interrupción externa para un pin específico
void hal_ext_int_habilitar_int(uint32_t channel) {
    
				for (uint32_t i = 0; i<BUTTONS_NUMBER; i++){
					if(boton_callbacks[i].auxData == channel){
					//if(boton_callbacks[i].pin == channel){
						NRF_GPIOTE->EVENTS_IN[boton_callbacks[i].canal_pin] = 0; // Limpiar el evento
						uint32_t dummy = NRF_GPIOTE->EVENTS_IN[boton_callbacks[i].canal_pin];
	
            // Habilitar la interrupción para este canal
            NRF_GPIOTE->INTENSET = (1 << boton_callbacks[i].canal_pin);
						NVIC_EnableIRQ(GPIOTE_IRQn);
					}
				}
    //

}

// Función para deshabilitar la interrupción externa para un pin específico
void hal_ext_int_deshabilitar_int(uint32_t channel) {
		NRF_GPIOTE->INTENCLR = (1 << channel);
		NVIC_DisableIRQ(GPIOTE_IRQn);
		
		/*for (uint32_t i = 0; i<BUTTONS_NUMBER; i++){
			if(boton_callbacks[i].auxData == channel){
			//if(boton_callbacks[i].pin == channel){
				NRF_GPIOTE->EVENTS_IN[boton_callbacks[i].canal_pin] = 0; // Limpiar el evento
				uint32_t dummy = NRF_GPIOTE->EVENTS_IN[boton_callbacks[i].canal_pin];
	
          // Habilitar la interrupción para este canal
				NRF_GPIOTE->INTENCLR = (1 << boton_callbacks[i].canal_pin);
				NVIC_DisableIRQ(GPIOTE_IRQn);
			}
		}*/

}

// Función para configurar el pin para despertar del modo de bajo consumo
void hal_ext_int_habilitar_despertar(uint32_t pin) {
	    // Configurar el pin para interrupciones externas (nivel bajo por defecto)
    NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) | 
														(GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos) |
														(GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);

}


// Función para preparar el dispositivo para entrar en modo de bajo consumo
void hal_ext_int_preparar_dormir() {
    // Configura el sistema para entrar en modo de apagado (sueño profundo)
    NRF_POWER->SYSTEMOFF = 1;  // Ordena al sistema entrar en modo de apagado
}

// Función para despertar el sistema del modo de bajo consumo
void hal_ext_int_despertar() {
    // Despierta el sistema del sueño profundo
    NRF_POWER->SYSTEMOFF = 0;  // Despierta el sistema
}

uint32_t hal_ext_int_leerINT(uint32_t gpio)
	{
	uint32_t x = (NRF_GPIO->IN >> gpio) & 0x01;
	return x ;// Desplazar y enmascarar para obtener el valor del pin específico
}