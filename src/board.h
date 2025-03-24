/* *****************************************************************************
 * P.H.2024: define la placa de desarrollo con la que estamos trabajando (pines, numero deleds, etc
 * forma parte del HAL
 */

#ifndef BOARD
#define BOARD

#if defined (LPC2105_simulador)
	#include "board_lpc.h"
#elif defined(BOARD_PCA10056)
	#include "board_nrf52840dk.h"
#elif defined(BOARD_PCA10059)
  #include "board_nrf52840_dongle.h"	
#else
	#error "Board is not defined"
#endif
#endif
