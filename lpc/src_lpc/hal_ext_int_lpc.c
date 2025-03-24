#include "hal_ext_int.h"
#include "board_lpc.h"
#include "drv_botones.h"
#include <LPC210x.h> 

static void (*ext_int_callback)(uint32_t pin);
//ext_int_callback = NULL;
//static EVENTO_T Id_evento_cb;
//static uint32_t auxData_cb;

void eint0_ISR(void) __irq {
		hal_ext_int_deshabilitar_int(14);
		EXTINT = 0x07; // Activa los bits 0, 1 y 2
		ext_int_callback(16);
    VICVectAddr = 0;
}

void eint1_ISR(void) __irq {
	hal_ext_int_deshabilitar_int(15);	
	EXTINT = 0x07; // Activa los bits 0, 1 y 2
  ext_int_callback(14);
	VICVectAddr = 0;
}

void eint2_ISR(void) __irq {
	hal_ext_int_deshabilitar_int(16);	
	EXTINT = 0x07; // Activa los bits 0, 1 y 2
  ext_int_callback(15);
  VICVectAddr = 0;
}

void hal_ext_int_iniciar(uint32_t pin, uint32_t auxData, void (*callback)(uint32_t auxData)) {
    ext_int_callback = callback;
		
		if (pin < 16){
			PINSEL0 = PINSEL0 | (0x02 << pin * 2); 
			IOCLR = (1 << pin);   
		} else {
			PINSEL1 = PINSEL1 | 0x01 << (pin - 16);        
			IOCLR = (1 << pin); 
		}
		switch(pin){
			case 14:
				VICVectAddr3 = (unsigned long)eint1_ISR; // Establece la dirección de la ISR para EINT1
				VICVectCntl3 = 0x20 | 15;             // Habilita el slot IRQ vectoreado para EINT1 (número 15)
				VICIntEnable |= (1 << 15);            // Habilita la interrupción EINT1 en el VIC
				break;
			case 15:
				VICVectAddr2 = (unsigned long)eint2_ISR; // Establece la dirección de la ISR para EINT2
				VICVectCntl2 = 0x20 | 16;             // Habilita el slot IRQ vectoreado para EINT1 (número 16)
				VICIntEnable |= (1 << 16);            // Habilita la interrupción EINT2 en el VIC
				break;
			case 16:
				VICVectAddr4 = (unsigned long)eint0_ISR; // Establece la dirección de la ISR para EINT0
				VICVectCntl4 = 0x20 | 14;             // Habilita el slot IRQ vectoreado para EINT0 (número 14)
				VICIntEnable |= (1 << 14);            // Habilita la interrupción EINT0 en el VIC
			break;
			default:
				break;
		}
}

void hal_ext_int_habilitar_int(uint32_t pin) {
    EXTINT = 0x07;
		if (pin == 16){
			VICIntEnable = (1 << 14);  // Habilita EINT1 (Interrupción externa 0)
		} else if(pin == 15 || pin == 14){
			VICIntEnable = (1 << (pin + 1));  // Habilita EINT1 (Interrupción externa 1, 2)
		} else {
			VICIntEnable = (1 << pin);  
		}
}

void hal_ext_int_deshabilitar_int(uint32_t pin) {

    VICIntEnClr = (1 << pin);  
}

void hal_ext_int_habilitar_despertar(uint32_t pin) {
    EXTWAKE |= (1 << pin);  
}

void hal_ext_int_deshabilitar_despertar(uint32_t pin) {
    EXTWAKE &= ~(1 << pin); 
}

void hal_ext_int_limpiarINT() {
		EXTINT = 0x0F;  
}

uint32_t hal_ext_int_leerINT(uint32_t pin) {
		uint32_t estado = EXTINT; // Leer el registro EXTINT
		if (pin == 16){
			if(estado & (1 << 0)){
				return 1;	 
			} else {
				return 0;
			}
		} else if(pin == 14){
			if(estado & (1 << 1)){
				return 1;	 
			} else {
				return 0;
			}
		}else if(pin == 15){
			if(estado & (1 << 2)){
				return 1;	 
			} else {
				return 0;
			}
		} else {
			return 0;
		}
}
// Manejador de la interrupción para EINT1
