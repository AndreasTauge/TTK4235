#include <stdint.h>

//BUTTONS1,4 = P0.13,16
#define __BUTTON_1_PIN__ 13
#define __BUTTON_2_PIN__ 14
#define __BUTTON_3_PIN__ 15
#define __BUTTON_4_PIN__ 16


//__GPIO_BASE_ADRESS0__ = P0, 0x50000000
#define __GPIO_BASE_ADDRESS__ 0x50000000

//0x700 - 0x524 = 0x1DC = 476 bytes = 119
#define __RESERVED1_SIZE__ 118

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
	// Chat seier (2 << 2) er riktig for å Enable pull-upp for BUTTON X
	// Flytter 11 til bitene 2 thru 4
	//PIN CNF only allows values 00,01,10
	// (2 << 2) enables pullupp / 0b1000
	GPIO->PIN_CNF[__BUTTON_1_PIN__] = (2 << 2);
	// Fill inn the configuration for the remaining buttons
	GPIO->PIN_CNF[__BUTTON_2_PIN__] = (2 << 2);
	GPIO->PIN_CNF[__BUTTON_3_PIN__] = (2 << 2);
	GPIO->PIN_CNF[__BUTTON_4_PIN__] = (2 << 2);
}

int main(){
	// Configure LED Matrix
	for(int i = 17; i <= 20; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}

	// Configure buttons -> see button_init()
	button_init();

	int sleep = 0;
	while(1){
	/* Check if button 1 is pressed;
	* turn on LED matrix if it is. */
	//int eller unit32?
	int button1_pressed = !(GPIO->IN & (1 << __BUTTON_1_PIN__));
	int button2_pressed = !(GPIO->IN & (1 << __BUTTON_2_PIN__));

	if (button1_pressed) {
	// Skru på LED-matrisen, flytter 1111 til bitene 17 thru 20
	GPIO->OUTCLR = (0b1111 << 17);
	}

	if (button2_pressed) {
	// Skru av LED-matrisen
	GPIO->OUTSET = (0b1111 << 17);
	}

	/* Check if button 2 is pressed;
	* turn off LED matrix if it is. */

		sleep = 10000;
		while(--sleep); // Delay
	}
	return 0;
}
