/* 
#include <stdint.h>
#include <stdbool.h>

#include "rt_fifo.h"

void test_cola(void){
	rt_FIFO_inicializar(4);
	for (int i = 0; i<64; i++){
		rt_FIFO_encolar(ev_T_PERIODICO, i);
	}	
	
}*/