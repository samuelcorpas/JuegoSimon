#ifndef HAL_CONSUMO_LPC_H
#define HAL_CONSUMO_LPC_H

// Inicialización del hardware relacionado con el consumo
void hal_consumo_iniciar(void);

// Poner el procesador en modo de espera (Idle)
void hal_consumo_esperar(void);

// Poner el procesador en modo de dormir (Power-down)
void hal_consumo_dormir(void);

#endif // HAL_CONSUMO_H
