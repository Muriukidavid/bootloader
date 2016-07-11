#include"MKL25Z4.h"
extern void init_clocks(void);
void delay(void);
void init_led(void);
void uart_putchar (uint8_t ch);
uint8_t uart_getchar (void);

void main(void){
	init_clocks();
	init_led();
	while(1){
		PTB_BASE_PTR->PTOR = 1 << 18;//toggle output
		delay();
		uart_putchar(0x31);
	}
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
//configure RED LED
void init_led(void){
	PORTB_PCR18 |= PORT_PCR_MUX(1);//alt1=gpio pin
	PTB_BASE_PTR->PDDR = 1 << 18;//as an output
}

//configure UART
void init_uart0(void){
	uint16_t ubd;
	//configure PTA1 & PTA2 as alt2=uart0
	PORTA_PCR1 |= PORT_PCR_MUX(2);
	PORTA_PCR2 |= PORT_PCR_MUX(2);
	PTA_BASE_PTR->PDDR = 1 << 2;//Tx as an output
	//disable reciever and transmitter
	UART_C2_REG(UART0_BASE_PTR) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
	//default settings 8N1
	UART0_C1 = 0;
	//9-bit mode disabled
	UART0_C3 = 0;
	UART0_S2 = 0;
	//calculate baudrate value
	ubd = (uint16_t)((48000*1000)/(115200 * 16));
	UART0_C4 = UARTLP_C4_OSR(16 - 1);
    UART0_BDH = (ubd >> 8) & UARTLP_BDH_SBR_MASK;
	UART0_BDL = (ubd & UARTLP_BDL_SBR_MASK);
	UART0_C2 = UARTLP_C2_RE_MASK | UARTLP_C2_TE_MASK | UART_C2_RIE_MASK;
}


void uart_putchar (uint8_t ch)
{
	/* Wait until space is available in the FIFO */
	//while(!(UART0_S1 & UART_S1_TDRE_MASK));

	/* Send the character */
	UART0_D = (uint8_t)ch;
}

uint8_t uart_getchar (void)
{
	/* Wait until character has been received */
	while (!(UART0_S1 & UART_S1_RDRF_MASK));

	/* Return the 8-bit data from the receiver */
	return (uint8_t)UART0_D;
}



