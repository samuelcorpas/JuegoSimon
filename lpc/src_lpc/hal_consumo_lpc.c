#include "hal_consumo.h"
#include <LPC210x.H>                       /* LPC210x definitions */

//definida en Startup.s
extern void switch_to_PLL(void);

/* inicia el hal de consumo */
void hal_consumo_iniciar(void) {
}

/* pone al procesador en estado de espera para reducir su consumo */
void hal_consumo_esperar(void)  {
  EXTWAKE = 7; 			// EXTINT0,1,2 will awake the processor
	PCON |= 0x01; 

}

/* duerme al procesador para minimizar su consumo */
void hal_consumo_dormir(void)  {
  EXTWAKE = 7; 			// EXTINT0,1,2 will awake the processor
	PCON |= 0x02; 
	switch_to_PLL(); 	//PLL aranca a 12Mhz cuando volvemos de power down ???????????
}
