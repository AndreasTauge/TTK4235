#include "uart.h"
#include "gpio.h"
#include <stdint.h>

#define UART_BASE 0x40002000
#define UART ((NRF_UART_REG*)UART_BASE)

typedef struct {
    volatile uint32_t TASKS_STARTRX;
    volatile uint32_t TASKS_STOPRX;
    volatile uint32_t TASKS_STARTTX;
    volatile uint32_t TASKS_STOPTX;
    volatile uint32_t RESERVED0[3];
    volatile uint32_t TASKS_SUSPEND;
    volatile uint32_t RESERVED1[56];

    volatile uint32_t EVENTS_CTS;
    volatile uint32_t EVENTS_NCTS;
    volatile uint32_t EVENTS_RXDRDY;
    volatile uint32_t RESERVED2[4];
    volatile uint32_t EVENTS_TXDRDY;
    volatile uint32_t RESERVED3;
    volatile uint32_t EVENTS_ERROR;
    volatile uint32_t RESERVED4[7];
    volatile uint32_t EVENTS_RXTO;
    volatile uint32_t RESERVED5[46];

    volatile uint32_t SHORTS;
    volatile uint32_t INTENSET;
    volatile uint32_t INTENCLR;
    volatile uint32_t ERRORSRC;
    volatile uint32_t ENABLE;
    volatile uint32_t RESERVED6[64];
    volatile uint32_t PSELRTS;
    volatile uint32_t PSELTXD;
    volatile uint32_t PSELCTS;
    volatile uint32_t PSELRXD;
    volatile uint32_t RXD;
    volatile uint32_t TXD;
    volatile uint32_t RESERVED7[93];
    volatile uint32_t BAUDRATE;
    volatile uint32_t RESERVED8;
    volatile uint32_t CONFIG;
    volatile uint32_t RESERVED9;
    volatile uint32_t RESERVED10;
    volatile uint32_t RESERVED11;
    volatile uint32_t RESERVED12[17];
} NRF_UART_REG;

#define UART_TX_PIN 6
#define UART_RX_PIN 8

void uart_init() {
    GPIO->PIN_CNF[UART_TX_PIN] = (0 << 0); // Output
    GPIO->PIN_CNF[UART_RX_PIN] = (1 << 0); // Input

    // Koble GPIO-pinnene til UART
    UART->PSELTXD = UART_TX_PIN;
    UART->PSELRXD = UART_RX_PIN;

    // Deaktiver RTS og CTS
    UART->PSELRTS = 0xFFFFFFFF;
    UART->PSELCTS = 0xFFFFFFFF;

    UART->BAUDRATE = 0x00275000; 

    UART->ENABLE = 4; 

    UART->TASKS_STARTRX = 1;
}

void uart_send(char letter){
    UART->TASKS_STARTTX = 1;
	UART->TXD = letter;
	while(!UART->EVENTS_TXDRDY);

	UART->EVENTS_TXDRDY = 0;
	UART->TASKS_STOPTX = 1;
}

char uart_read(){

	if(!UART->EVENTS_RXDRDY) {
        return '\0';
    }

    UART->EVENTS_RXDRDY = 0;

    return (char)UART->RXD;
}

void uart_send_str(char ** str){
	UART->TASKS_STARTTX = 1;
	char * letter_ptr = *str;
	while(*letter_ptr != '\0'){
		UART->TXD = *letter_ptr;
		while(!UART->EVENTS_TXDRDY);
		UART->EVENTS_TXDRDY = 0;
		letter_ptr++;
}
}