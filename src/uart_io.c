#include "uart_io.h"

void uart_init(int baud)
{
	Uart* uart = UART_BASE;
	Iomux* iomux = IOMUX_BASE;

	iomux->u0_txd = F_U0TXD;

	uart->autobaud = 0;
	/* 8n1 */
	uart->conf0 = C_STOP1 | C_8BIT;
	
	uart->clkdiv = UART_CLOCK / baud;
	uart_fifo_reset();
}

// Resets Queue
void uart_fifo_reset(void)
{
	Uart* uart = UART_BASE;

	uart->conf0 |= C_TX_RESET | C_RX_RESET;
	uart->conf0 &= ~(C_TX_RESET | C_RX_RESET);
}

void uart_putchar(unsigned char c)
{
	Uart* uart = UART_BASE;

	while (uart->status & ST_TX_MASK);
	uart->fifo = (unsigned int) c;
}

unsigned char uart_getchar()
{
	Uart* uart = UART_BASE;
	
	while (uart->status & ST_RX_MASK);
	return uart->fifo;
}