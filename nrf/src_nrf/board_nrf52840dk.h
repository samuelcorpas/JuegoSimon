#ifndef BOARD_NRF
#define BOARD_NRF

// BOARD_PCA10056

/** @brief Macro for mapping port and pin numbers to values understandable for nrf_gpio functions. */
#define NRF_GPIO_PIN_MAP(port, pin) (((port) << 5) | ((pin) & 0x1F))

// LEDs definitiones para nRF52480 DK, PCA10056 en nRF SDK
#define LEDS_NUMBER    4

#define LED_1          NRF_GPIO_PIN_MAP(0,13)
#define LED_2          NRF_GPIO_PIN_MAP(0,14)
#define LED_3          NRF_GPIO_PIN_MAP(0,15)
#define LED_4          NRF_GPIO_PIN_MAP(0,16)

#define LEDS_ACTIVE_STATE 0

#define LEDS_LIST { LED_1, LED_2, LED_3, LED_4 }

// Botones
#define BUTTONS_NUMBER 4

#define BUTTON_1       NRF_GPIO_PIN_MAP(0,11)
#define BUTTON_2       NRF_GPIO_PIN_MAP(0,12)
#define BUTTON_3       NRF_GPIO_PIN_MAP(0,24)
#define BUTTON_4       NRF_GPIO_PIN_MAP(0,25)

#define BUTTON_PULL    1 //poner pullup interno a los botones

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST { BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4 }

// MONITORES
#define MONITOR_NUMBER 4

#define MONITOR1       NRF_GPIO_PIN_MAP(0,28)
#define MONITOR2       NRF_GPIO_PIN_MAP(0,29)
#define MONITOR3       NRF_GPIO_PIN_MAP(0,30)
#define MONITOR4       NRF_GPIO_PIN_MAP(0,31)

#define MONITOR_ACTIVE_STATE 0

#define MONITOR_LIST { MONITOR1, MONITOR2, MONITOR3, MONITOR4 }

#endif
