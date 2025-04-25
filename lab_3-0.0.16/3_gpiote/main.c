#include "gpio.h"
#include "gpiote.h"
#include "ppi.h"

int main(void)
{
    gpio_init();
    gpiote_init();
    ppi_init();

    while (1) {
        __WFE();        
    }
}
