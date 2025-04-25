#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h> 

//BUTTONS1,4 = P0.13,16
#define __BUTTON_1_PIN__ 13
#define __BUTTON_2_PIN__ 14
#define __BUTTON_3_PIN__ 15
#define __BUTTON_4_PIN__ 16


//__GPIO_BASE_ADRESS0__ = P0, 0x50000000
#define __GPIO_BASE_ADDRESS__ 0x50000000

//0x700 - 0x524 = 0x1DC = 476 bytes = 119
#define __RESERVED1_SIZE__ 118

static const uint8_t led_cols[4] = {17, 18, 19, 20};

#define GPIO ((NRF_GPIO_REGS*)__GPIO_BASE_ADDRESS__)

typedef struct {
	volatile uint32_t RESERVED0[321];
	volatile uint32_t OUT;
	volatile uint32_t OUTSET;
	volatile uint32_t OUTCLR;
	volatile uint32_t IN;
	volatile uint32_t DIR;
	volatile uint32_t DIRSET;
	volatile uint32_t DIRCLR;
	volatile uint32_t LATCH;
	volatile uint32_t DETECTMODE;
	volatile uint32_t RESERVED1[__RESERVED1_SIZE__];
	volatile uint32_t PIN_CNF[32];
} NRF_GPIO_REGS;

void button_init(){ 
	// Flytter 11 til bitene 2 thru 4

	GPIO->PIN_CNF[__BUTTON_1_PIN__] = (3 << 2);
	GPIO->PIN_CNF[__BUTTON_2_PIN__] = (3 << 2);
	GPIO->PIN_CNF[__BUTTON_3_PIN__] = (3 << 2);
	GPIO->PIN_CNF[__BUTTON_4_PIN__] = (3 << 2);

	for(int i=0; i<4; i++) {
		GPIO->DIRSET = (1 << led_cols[i]);
		GPIO->OUTCLR = (1 << led_cols[i]);
	}
}

ssize_t _write(int fd, const void *buf, size_t count){
	char * letter = (char *)(buf);
	for(int i = 0; i < count; i++){
	uart_send(*letter);
	letter++;
	}
	return count;
}

int main () {

	uart_init();
	button_init();
	int leds_on = 0;

	iprintf("The average grade in TTK%d was in %d was: %c\n\r", 4235, 2022, 'B');


	while(1){
		int button1_pressed = !(GPIO->IN & (1 << __BUTTON_1_PIN__));
		int button2_pressed = !(GPIO->IN & (1 << __BUTTON_2_PIN__));
		if(button1_pressed){
			uart_send('A');
		}
		else if(button2_pressed){
			uart_send('B');
		}


		if (uart_read() != '\0'){
			if(	leds_on ){
				GPIO->OUTCLR = (0b1111 << 17);
				leds_on = 0;
			}
			else if ( !leds_on ){
				GPIO->OUTSET = (0b1111 << 17);
				leds_on = 1;
			}
		}
	}
}

