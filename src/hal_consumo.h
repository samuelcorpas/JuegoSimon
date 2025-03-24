#ifndef HAL_CONSUMO_NRF_H
#define HAL_CONSUMO_NRF_H

// Inicializaci�n del hardware relacionado con el consumo
void hal_consumo_iniciar(void);

// Poner el procesador en modo de espera (WFI)
void hal_consumo_esperar(void);

// Poner el procesador en modo de dormir (funci�n pendiente de implementaci�n futura)
void hal_consumo_dormir(void);

#endif // HAL_CONSUMO_NRF_H
