#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "concurrencia.h"

static uint32_t m_in_critical_region = 0;
static uint32_t cpsr;

extern void __Disable_irq(void);
extern void __Enable_irq(void);


/*void __Disable_irq() {
    __asm volatile ("mrs r0, cpsr");      // Leer CPSR en r0
    __asm volatile ("orr r1, r0, #0xC0"); // Establecer los bits IRQ (I) y FIQ (F)
    __asm volatile ("msr cpsr_c, r1"
										:	"=r"(cpsr)       // Salida: r0 almacena el CPSR actual en cpsr
										:
										: "r1"          
			);    // Escribir el nuevo CPSR            
}


void __enable_irq() {
    __asm volatile ("msr cpsr_c, r0"
										:
										: "r" (cpsr)        // Entrada: %0 es el valor guardado en saved_cpsr
										:
		);
}*/

uint32_t SC_entrar_disable_irq(void) {
		if (m_in_critical_region == 0) {
       __Disable_irq();  // Desactiva interrupciones solo si no está en una sección crítica
    }	
		return(m_in_critical_region++);
}


void SC_salir_enable_irq(void) {
		
	if (m_in_critical_region > 0) {
        m_in_critical_region--;
        if (m_in_critical_region == 0) {
            __Enable_irq();  // Restaura el estado previo solo si ya no hay secciones críticas activas
        }
    }
}



