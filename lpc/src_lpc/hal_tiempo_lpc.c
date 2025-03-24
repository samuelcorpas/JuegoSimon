/* *****************************************************************************
 * P.H.2024: Temporizadores en LPC2105, Timer 0 y Timer 1
 * implementacion para cumplir el hal_tiempo.h
 * Timer0 cumple maxima frecuencia, minimas interrupciones contando en ticks
 * Timer1 avisa cada periodo de activacion en ticks
 */
 
#include <LPC210x.H> /* LPC210x definitions */
#include <stdint.h>

#define MAX_COUNTER_VALUE 0xFFFFFFFE			// Maximo valor del contador de 32 bits
#define HAL_TICKS2US			15 							// funcionamos PCLK a 15 MHz de un total de 60 MHz CPU Clock
//#define US2MS							1000						//milisegundos por microsogundos

/* *****************************************************************************
 * Timer0 contador de ticks
 */

static volatile uint32_t timer0_int_count = 0;	// contador de 32 bits de veces que ha saltado la RSI Timer0

/* *****************************************************************************
 * Timer 0 Interrupt Service Routine
 */
void timer0_ISR (void) __irq {
    timer0_int_count++;
    T0IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                       // Acknowledge Interrupt
}

/* *****************************************************************************
 * Programa un contador de tick sobre Timer0, con maxima precisión y minimas interrupciones
 */
uint32_t hal_tiempo_iniciar_tick() {
	timer0_int_count = 0;	
	T0MR0 = MAX_COUNTER_VALUE;		// Si TC = 1 interrupcion cada us
  T0MCR = 3;              			// Generates an interrupt and resets the count when the value of MR0 is reached
	
  // configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
	VICVectAddr0 = (unsigned long)timer0_ISR;    // set interrupt vector in 0
  // 0x20 bit 5 enables vectored IRQs. 
	// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
	VICVectCntl0 = 0x20 | 4;                   
  VICIntEnable = VICIntEnable | 0x00000010;       // Enable Timer0 Interrupt

	T0TCR = 2; // Reincia los contadores
  T0TCR = 1;  // Empieza la cuenta
	
	return HAL_TICKS2US;  		//devuelve el factor conversion de ticks a microsegundos de este hardware
}


/**
 * Lee el tiempo que lleva contando el contador y lo devuelve en ticks.
 */
uint64_t hal_tiempo_actual_tick() { 
	uint64_t time;
  time = ((MAX_COUNTER_VALUE+1)*timer0_int_count) + (uint64_t)T0TC; 
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
void timer1_ISR (void) __irq {
	f_callback();													 // Llamo a la función desde la RSI.
	T1IR = 1;                              // Clear interrupt flag
	VICVectAddr = 0;                       // Acknowledge Interrupt
}

/* *****************************************************************************
 * Dependiente del hardware Timer1
 * Programa el reloj para que llame a la función de callback cada periodo.
 * El periodo se indica en tick. Si el periodo es cero se para el temporizador. 
 */
void hal_tiempo_reloj_periodico_tick(uint32_t periodo_en_tick, void(*funcion_callback)()){
  
	f_callback = funcion_callback;
	
	if (periodo_en_tick != 0) { //Si el periodo es cero solo se para el temporizador.
		T1MR0 = periodo_en_tick  - 1; 		// 15 Ticks (ciclos) por microsegundo.
																			// (periodo_en_ms * HAL_TICKS2US * US2MS) - 1;  
																			// resto uno por como incrementa y compara
		T1MCR = 3;  
		// Enable Timer1 Interrupt.
		VICVectAddr1 = (unsigned long)timer1_ISR;
		// 0x20 bit 5 enables vectored IRQs. 
		// 5 is the number of the interrupt assigned. Number 5 is the Timer 1
		VICVectCntl1 = 0x20 | 5;                   
		VICIntEnable = VICIntEnable | 0x00000020;  
		
		T1TCR = 3;  // Reincia los contadores
		T1TCR = 1;  // Empieza la cuenta
	} else {
		// Detiene el temporizador
		T1TCR = 0;  
    VICIntEnClr = 0x20;                     // Disable Timer1 Interrupt		
	}
}


