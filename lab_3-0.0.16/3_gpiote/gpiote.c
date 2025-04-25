#include "gpiote.h"


#define BTN1_PIN   13            


static const uint8_t led_sup[4] = { 17, 18, 19, 20 };  


void gpiote_init(void)
{              

    // task
    for (int k = 0; k < 4; ++k) {
        GPIOTE->CONFIG[k] =
            (led_sup[k] & 0x1F)      |
            (3 << 0)       |
            (3 << 16)      |
            (0 << 20); 
    }

    // event
    GPIOTE->CONFIG[4] =
        (BTN1_PIN & 0x1F)            |       
        (1 << 0)           |       
        (2 << 16);  
}
