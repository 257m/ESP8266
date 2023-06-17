#include "uart_io.h"

void uart_init(int baud)
{
	Uart* uart = UART_BASE;
	Iomux* iomux = IOMUX_BASE;

	// Stop TX from multiplexing as I need it for UART not GPIO
	iomux->u0_txd = F_U0TXD;

	// Turn off autobaud as I will set it manually
	uart->autobaud = 0;
	// Turn on 8 bit mode and use 1 bit to signal the end of a frame
	uart->conf0 = C_STOP1 | C_8BIT; /* 8n1 */
	// Set clock divider	
	uart->clkdiv = UART_CLOCK / baud;
	// Reset the fifo queue because we will need it for UART
	uart_fifo_reset();
}

// Resets Queue
void uart_fifo_reset(void)
{
	Uart* uart = UART_BASE;

	// Flash UART reset pins
	uart->conf0 |= C_TX_RESET | C_RX_RESET;
	uart->conf0 &= ~(C_TX_RESET | C_RX_RESET);
}

/*void uart_putchar(unsigned char c)
{
	Uart* uart = UART_BASE;
	// Wait for ongoing UART processes to finish
	while (uart->status & ST_TX_MASK);
	// Add the character to the fifo queue
	uart->fifo = (unsigned int) c;
}*/

unsigned char uart_getchar()
{
	Uart* uart = UART_BASE;

	while (!(uart->status & ST_RX_MASK));
	return uart->fifo;
}

/*void uart_memcpy(unsigned char* dest, unsigned int len)
{
	while (len--)
		*dest = uart_getchar();
}*/
