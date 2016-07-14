#include "gpio.h"
//configure RED LED
void init_led(void){
	PORTB_PCR18 |= PORT_PCR_MUX(1);//alt1=gpio pin
	PTB_BASE_PTR->PDDR |= (uint32_t)(1<<18);//as an output
	PTB_BASE_PTR->PDOR &= ~(uint32_t)(1<<18);
}

//delay
void delay(void)
{
  volatile unsigned int i,j;

  for(i=0; i<1000; i++)
  {
	for(j=0; j<300; j++)
      __asm__("nop");
  }
}
