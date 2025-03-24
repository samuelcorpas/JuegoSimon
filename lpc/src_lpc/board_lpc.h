/* *****************************************************************************
 * P.H.2024: definicion de la placa simulada del LPC2105 en Keil
 */

#ifndef BOARD_LPC
#define BOARD_LPC

#include <LPC210x.H>                       /* LPC210x definitions */

#include "reserva_gpio_lpc2105.h"

// LEDs definitions for LPC2105 simulado

#define LEDS_NUMBER    4

#define LED_1          (LED1_GPIO)
#define LED_2          (LED2_GPIO)
#define LED_3          (LED3_GPIO)
#define LED_4          (LED4_GPIO)

#define LEDS_ACTIVE_STATE 1

#define LEDS_LIST { LED_1, LED_2, LED_3, LED_4 }

//botones
#define BUTTONS_NUMBER 3

#define BUTTON_1       (INT_EXT1)
#define BUTTON_2       (INT_EXT2)
#define BUTTON_3       (INT_EXT3)

#define BUTTONS_LIST { BUTTON_1, BUTTON_2, BUTTON_3}


//MONITOR
#define MONITOR_NUMBER 4

#define MONITOR1       (MONITOR1_GPIO)
#define MONITOR2       (MONITOR2_GPIO)
#define MONITOR3       (MONITOR3_GPIO)
#define MONITOR4       (MONITOR4_GPIO)

#define MONITOR_ACTIVE_STATE 1

#define MONITOR_LIST { MONITOR1, MONITOR2, MONITOR3, MONITOR4 }

#endif
