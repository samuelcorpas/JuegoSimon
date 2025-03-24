/* *****************************************************************************
 * P.H.2024: TODO
 * implementacion para cumplir el hal_tiempo.h
 */
 
#include <nrf.h>
#include <stdint.h>

#define MAX_COUNTER_VALUE 0xFFFFFFFE			// Maximo valor del contador de 32 bits
#define HAL_TICKS2US			16 							// funcionamos PCLK a 16 MHz 
//#define US2MS							1000						//milisegundos por microsogundos

/* *****************************************************************************
 * Timer0 contador de ticks
 */

static volatile uint32_t timer0_int_count = 0;	// contador de 32 bits de veces que ha saltado la RSI Timer0

/* *****************************************************************************
 * Timer 0 Interrupt Service Routine
 */
void TIMER0_IRQHandler (void){
	volatile uint32_t dummy;
    if (NRF_TIMER0->EVENTS_COMPARE[0] == 1)
  {
    NRF_TIMER0->EVENTS_COMPARE[0] = 0;

    // Read back event register so ensure we have cleared it before exiting IRQ handler.
    dummy = NRF_TIMER0->EVENTS_COMPARE[0];
    dummy; // to get rid of set but not used warning
  }
	
	timer0_int_count++;
    // Limpiar la interrupción pendiente en el NVIC
    NVIC_ClearPendingIRQ(TIMER0_IRQn);                // Acknowledge Interrupt
}

/* *****************************************************************************
 * Programa un contador de tick sobre Timer0, con maxima precisión y minimas interrupciones
 */
uint32_t hal_tiempo_iniciar_tick() {
	// Reiniciar el contador de interrupciones del Timer 0
    timer0_int_count = 0;

    // Apagar el temporizador antes de configurarlo
    NRF_TIMER0->TASKS_STOP = 1;

    // Configurar el temporizador en modo TIMER (no en modo COUNTER)
    NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer; // 0 = Timer Mode

    // Configurar el temporizador con una resolución de 32 bits
    NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;

    // Configurar la frecuencia del temporizador (ejemplo: 1 MHz = 1 tick por microsegundo)
    NRF_TIMER0->PRESCALER = 0; // 1 MHz (16 MHz / 2^0 = 16 MHz)

    // Establecer el valor de comparación en el registro CC[0]
    NRF_TIMER0->CC[0] = MAX_COUNTER_VALUE;
	

    // Configurar el evento de comparación (Match Compare) para reiniciar y generar interrupción
    NRF_TIMER0->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos;
    NRF_TIMER0->INTENSET = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos;

    // Limpiar cualquier evento de comparación previo
    NRF_TIMER0->EVENTS_COMPARE[0] = 0;

    // Habilitar la interrupción del Timer 0 en el NVIC
    NVIC_EnableIRQ(TIMER0_IRQn);

    // Reiniciar y luego comenzar el temporizador
    NRF_TIMER0->TASKS_CLEAR = 1;
    NRF_TIMER0->TASKS_START = 1;

    return HAL_TICKS2US;   		//devuelve el factor conversion de ticks a microsegundos de este hardware
}


/**
 * Lee el tiempo que lleva contando el contador y lo devuelve en ticks.
 */
uint64_t hal_tiempo_actual_tick() {
    uint64_t time;
		// Obtener el valor actual del contador del Timer 0
		NRF_TIMER0->TASKS_CAPTURE[1] = 1;
    uint32_t contador_actual = NRF_TIMER0->CC[1];  // Usamos el registro de comparación CC[1] como referencia
    
    // Combinar las interrupciones y el valor actual del contador
    time = ((uint64_t)(MAX_COUNTER_VALUE) * (uint64_t)timer0_int_count) + (uint64_t)contador_actual;

    return time;
}

/* *****************************************************************************
 * Activacion periodica con timer 1
 */
static void(*f_callback)();		//puntero a funcion a llamar cuando salte la RSI (en modo irq)

/* *****************************************************************************
 * Timer 1 Interrupt Service Routine
 * llama a la funcion de callbak que se ejecutara en modo irq
 */
void TIMER1_IRQHandler(void) {
			volatile uint32_t dummy;
    if (NRF_TIMER1->EVENTS_COMPARE[0] != 0) {  // Verifica si el evento de comparación ocurrió
        NRF_TIMER1->EVENTS_COMPARE[0] = 0;     // Limpia el evento de comparación
			 dummy = NRF_TIMER1->EVENTS_COMPARE[0];
			 dummy; // to get rid of set but not used warning

        f_callback();  // Llama a la función callback
    }
}

/* *****************************************************************************
 * Dependiente del hardware Timer1
 * Programa el reloj para que llame a la función de callback cada periodo.
 * El periodo se indica en tick. Si el periodo es cero se para el temporizador. 
 */
void hal_tiempo_reloj_periodico_tick(uint32_t periodo_en_tick, void(*funcion_callback)()){
  
	f_callback = funcion_callback;
	
	if (periodo_en_tick != 0) { //Si el periodo es cero solo se para el temporizador.
		// Apagar el temporizador antes de configurar
        NRF_TIMER1->TASKS_STOP = 1;

        // Resetear el contador
        NRF_TIMER1->TASKS_CLEAR = 1;

        // Configurar el modo de temporizador
        NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;  // Modo Timer
        NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_32Bit;  // Temporizador de 32 bits

        // Configurar la prescaler si es necesario (para ajustarse a la frecuencia deseada)
        // NRF_TIMER1->PRESCALER = 4; // Ejemplo de configuración de prescaler (usualmente 16 MHz / 2^(prescaler))

        // Configurar el valor de comparación (periodo en ticks)
        NRF_TIMER1->CC[0] = periodo_en_tick ;
				NRF_TIMER1->PRESCALER = 0;

        // Configurar para generar interrupciones en el evento de comparación
        NRF_TIMER1->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Enabled;  // Reinicia el contador al alcanzar CC[0]
        NRF_TIMER1->INTENSET = TIMER_INTENSET_COMPARE0_Msk;    // Habilitar interrupciones en CC[0]

        // Habilitar la interrupción en el NVIC

        NVIC_EnableIRQ(TIMER1_IRQn);

        // Iniciar el temporizador
				NRF_TIMER1->TASKS_CLEAR = 1;
        NRF_TIMER1->TASKS_START = 1;
	} else {
		// Detiene el temporizador
		    NRF_TIMER1->TASKS_STOP = 1;
			//	NRF_TIMER1->INTENCLR = TIMER_INTENSET_COMPARE0_Msk;
        //NVIC_DisableIRQ(TIMER1_IRQn);	
	}
}
