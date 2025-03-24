/* *****************************************************************************
 * P.H.2024: Configuracion y reserva de pines y gpios en el simulador del LPC2105 en Keil
 */
 
#ifndef RESERVA_GPIO
#define RESERVA_GPIO

//gpios - pines

//LEDS
enum { 	LED1_GPIO = 0,
				LED2_GPIO = 1,
				LED3_GPIO = 2,
				LED4_GPIO = 3,
};

//BOTONES
enum {  INT_EXT1 = 14, //eINT1
				INT_EXT2 = 15, //eINT2
				INT_EXT3 = 16, //eINT0
};
// MONITORES
enum { 	MONITOR1_GPIO = 28,
				MONITOR2_GPIO = 29,
				MONITOR3_GPIO = 30,
				MONITOR4_GPIO = 31,
};
#endif
