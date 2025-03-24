#include <stdint.h>
#include <stdbool.h>
#include <string.h>
//Funcion para entrar en seccion critica
uint32_t SC_entrar_disable_irq(void);


//Funcion para salir de seccion critica
void SC_salir_enable_irq(void);

