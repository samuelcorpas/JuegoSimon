#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hal_concurrencia.h"

static uint32_t m_in_critical_region = 0;
//static uint32_t cpsr;

/*extern void __Disable_irq(void);
extern void __Enable_irq(void);*/


uint32_t SC_entrar_disable_irq(void) {
		if (m_in_critical_region == 0) {
       __disable_irq();  // Desactiva interrupciones solo si no está en una sección crítica
    }	
		return(m_in_critical_region++);
}


void SC_salir_enable_irq(void) {
		
	if (m_in_critical_region > 0) {
        m_in_critical_region--;
        if (m_in_critical_region == 0) {
            __enable_irq();  // Restaura el estado previo solo si ya no hay secciones críticas activas
        }
    }
}



