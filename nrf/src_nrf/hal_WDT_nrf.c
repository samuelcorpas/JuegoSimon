#include <nrf.h>
#include "hal_WDT.h"

void hal_WDT_iniciar(uint32_t sec) {
    //NRF_WDT->TASKS_STOP = 1;


		NRF_WDT->CRV = 32768 * sec; 
		NRF_WDT->RREN |= WDT_RREN_RR0_Msk;  

    // Configurar el comportamiento del WDT (reinicio del sistema cuando expire)
		NRF_WDT->CONFIG = (WDT_CONFIG_HALT_Pause << WDT_CONFIG_HALT_Pos) | ( WDT_CONFIG_SLEEP_Run << WDT_CONFIG_SLEEP_Pos);   

    NRF_WDT->TASKS_START = 1;
		hal_WDT_feed();

}

void hal_WDT_feed(void) {
    NRF_WDT->RR[0] = WDT_RR_RR_Reload; 
    NRF_WDT->RR[1] = WDT_RR_RR_Reload;  
}
