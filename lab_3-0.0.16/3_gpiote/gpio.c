#include "gpio.h"

#define BTN1_PIN   13            


static const uint8_t led_sup[4] = { 17, 18, 19, 20 };   


void gpio_init(void)
{
    GPIO->PIN_CNF[BTN1_PIN] = (3 << 2);

    for (int i = 0; i < 4; ++i) {
        GPIO->PIN_CNF[led_sup[i]] = (1 << 0);     
        GPIO->OUTCLR = (1 << led_sup[i]);
    }
}
