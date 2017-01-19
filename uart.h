#include"ring.h"
// Circular transmit and recieve buffers
#define BUFLEN 128

static uint8_t _tx_buffer[sizeof(RingBuffer) + BUFLEN] __attribute__ ((aligned(4)));
static uint8_t _rx_buffer[sizeof(RingBuffer) + BUFLEN] __attribute__ ((aligned(4)));

static RingBuffer *const tx_buffer = (RingBuffer *) &_tx_buffer;
static RingBuffer *const rx_buffer = (RingBuffer *) &_rx_buffer;

void uart_putchar (uint8_t ch);
void init_uart0(void);
//void uart_puts(uint8_t *s);
int uart_write(uint8_t *p, uint8_t len);
int uart_read(char *p, int len);
