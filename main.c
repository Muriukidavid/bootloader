/*
* main.c
* David Muriuki Karibe
*
* Licence: check LICENCE file
* July 2016
*/
#include "gpio.h"
#include "uart.h"

#define disable_irq(void) __asm__("CPSID i")
#define enable_irq(void) __asm__("CPSIE i")

extern void init_clocks(void);

uint8_t data_available, data;

void main(void){
	data_available = 0;
	data = 0;
	disable_irq();
	init_clocks();
	init_led();
	init_uart0();
	enable_irq();
	while(1){
		PTB_BASE_PTR->PTOR = 1 << 18;//toggle output
		delay();
		if(data_available){
			data_available=0;
			uart_putchar(data);
		}
	}
}









