#include "gpio.h"
#include "uart.h"

#define disable_irq(void) __asm__("CPSID i")
#define enable_irq(void) __asm__("CPSIE i")
#define RED 18

extern void init_clocks(void);

uint8_t data[100];

void init_system(void){
	disable_irq();
	init_clocks();
	init_RED_LED();
	init_uart0();
	enable_irq();
}
