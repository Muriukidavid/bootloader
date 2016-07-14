#include "uart.h"
//configure UART
void init_uart0(void){
	uint16_t ubd;
	//configure PTA1 & PTA2 as alt2=uart0
	PORTA_PCR1 |= PORT_PCR_MUX(2);
	PORTA_PCR2 |= PORT_PCR_MUX(2);
	PTA_BASE_PTR->PDDR |= (uint32_t)(1<<2);//Tx as an output, Rx is input by default
	//disable reciever and transmitter
	UART0_C2 =0;
	//default settings 8N1
	UART0_C1 = 0;
	//9-bit mode disabled
	UART0_C3 = 0;
	UART0_S2 = 0;
	//calculate baudrate value
	ubd = (uint16_t)((48000*1000)/(57600 * 16));
	UART0_C4 = UARTLP_C4_OSR(16 - 1);
    UART0_BDH = (ubd >> 8) & UARTLP_BDH_SBR_MASK;
	UART0_BDL = (ubd & UARTLP_BDL_SBR_MASK);
	UART0_C2 |= (uint32_t)(UARTLP_C2_RE_MASK | UARTLP_C2_TE_MASK | UARTLP_C2_RIE_MASK);
	//enable reciever interrupts
	NVIC_ICPR |= (uint32_t)(1<<(INT_UART0-16));//clear pending UART0_RX interrupts
	NVIC_ISER |= (uint32_t)(1<<(INT_UART0-16));//enable UART0 interrupts
}


void uart_putchar (uint8_t ch)
{
	/* Wait until space is available in the FIFO */
	while(!(UART0_S1 & (1<<8))==(1<<8));

	/* Send the character */
	UART0_D = (uint8_t)ch;
}

//UART0 brief interrupt handler
void UART0_IRQHandler(){
	if((UART0_S1 & UART_S1_RDRF_MASK)==UART_S1_RDRF_MASK){
		data = (uint8_t)UART0_D;
		data_available=1;
	}
}
