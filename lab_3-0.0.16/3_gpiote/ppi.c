#include "ppi.h"
#include "gpiote.h"

void ppi_init(void)
{
    // set event adress 
    uint32_t btn_evt = (uint32_t)&GPIOTE->EVENTS_IN[4];

    // set one channel adress for each led and enable it 
    for (int ch = 0; ch < 4; ++ch) {
        uint32_t led_task = (uint32_t)&GPIOTE->TASKS_OUT[ch];

        PPI->PPI_CH[ch].EEP = btn_evt;   
        PPI->PPI_CH[ch].TEP = led_task;  
        PPI->CHENSET = (1 << ch);       
    }
}
