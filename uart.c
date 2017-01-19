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
	//set oversampling ratio to 16 times
	UART0_C4 = UARTLP_C4_OSR(16 - 1);
    UART0_BDH = (ubd >> 8) & UARTLP_BDH_SBR_MASK;
	UART0_BDL = (ubd & UARTLP_BDL_SBR_MASK);
	
	// Initialize transmit and receive circular buffers
    buf_reset(tx_buffer, BUFLEN);
	buf_reset(rx_buffer, BUFLEN);

	//enable reciever, transmitter and reciever interrupts
	UART0_C2 |= (uint32_t)(UARTLP_C2_RE_MASK | UARTLP_C2_TE_MASK | UARTLP_C2_RIE_MASK);
	//enable UART0 interrupts
	NVIC_ICPR |= (uint32_t)(1<<(INT_UART0-16));//clear pending UART0 interrupts
	NVIC_ISER |= (uint32_t)(1<<(INT_UART0-16));//enable UART0 interrupts
}

// a single character send
void uart_putchar (uint8_t ch){
	/* Wait until space is available in the FIFO */
	while(!(UART0_S1 & (1<<8))==(1<<8));
	/* Send the character */
	UART0_D = (uint8_t)ch;
}

int uart_write(uint8_t *p, uint8_t len){
    uint8_t i;
    
    for(i=0; i<len; i++) {
        while(buf_isfull(tx_buffer)); // Spin wait while full
            
        buf_put_byte(tx_buffer, *p++);
        UART0_C2 |= UART_C2_TIE_MASK; // Turn on Tx interrupts
    }
    return len;
}

int uart_read(char *p, int len){
    int i = len;

    while(i > 0) {
        while(buf_isempty(rx_buffer)); // Spin wait
            
        *p++ = buf_get_byte(rx_buffer);
        UART0_C2 |= UART_C2_RIE_MASK; // Turn on Rx interrupt
        i--;
    }
    return len - i;
}


//UART0 brief interrupt handler
void UART0_IRQHandler(){
	//if there is data in the transmit buffer and the TDR is empty
	if((UART0_S1 & UART_S1_TDRE_MASK) && !buf_isempty(tx_buffer)){
		UART0_D = buf_get_byte(tx_buffer);
		if(buf_isempty(tx_buffer)){ //if tx_buffer is empty, 
			UART0_C2 &= ~UART_C2_TIE_MASK; //disable transmit interrupts
		}
	}
	//if the recieve buffer is not full and RDR is full
	if((UART0_S1 & UART_S1_RDRF_MASK ) && !buf_isfull(rx_buffer)){
		buf_put_byte(rx_buffer, UART0_D);
		if(buf_isfull(rx_buffer)){
			UART0_C2 &= ~UART_C2_RIE_MASK; //disable recieve interrupts
		}
	}
}
