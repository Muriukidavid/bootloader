#include "gpio.h"
//configure RED LED
void init_RED_LED(void){
	PORTB_PCR18 |= PORT_PCR_MUX(1);//alt1=gpio pin
	PTB_BASE_PTR->PDDR |= (uint32_t)(1<<18);//as an output
	PTB_BASE_PTR->PDOR &= ~(uint32_t)(1<<18);
}

void blink_RED_LED(void){
	PTB_BASE_PTR->PTOR = 1 << 18;//toggle output
	delay();
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
