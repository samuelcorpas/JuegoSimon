#ifndef HAL_CONSUMO_NRF_H
#define HAL_CONSUMO_NRF_H

// Inicialización del hardware relacionado con el consumo
void hal_consumo_iniciar(void);

// Poner el procesador en modo de espera (WFI)
void hal_consumo_esperar(void);

// Poner el procesador en modo de dormir (función pendiente de implementación futura)
void hal_consumo_dormir(void);

#endif // HAL_CONSUMO_NRF_H
