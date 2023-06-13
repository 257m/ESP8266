#include "uart_io.h"

// I put a bit of macro redundancy here
// but it is neccessary if the addresses change
// and the code has to be ported over to other
// devices. This code should be portable to most
// Atmega chips/Arduino Boards
#define UCSRA (*(&UCSR0A + 0x8*uart_selected))
#define UCSRB (*(&UCSR0B + 0x8*uart_selected))
#define UCSRC (*(&UCSR0C + 0x8*uart_selected))
#define UBRRH (*(&UBRR0H + 0x8*uart_selected))
#define UBRRL (*(&UBRR0L + 0x8*uart_selected))
#define UDR (*(&UDR0 + 0x8*uart_selected))

#define U2X U2X0
#define UCSZ0 UCSZ00
#define UCSZ1 UCSZ01
#define RXEN RXEN0
#define TXEN RXEN0
#define TXC TXC0
#define RXC RXC0
#define UDRE UDRE0
#define USBS USBS0
#define RCIE RCIE0

// Sadly I need to use static state unless
// I want to pass in the UART select for every function
static unsigned char uart_selected = 0;

inline void uart_select(unsigned char selected)
{
  uart_selected = selected;
}

inline bool uart_available()
{
	return UCSRA & _BV(RXC);
}

void uart_init(unsigned long baud_rate)
{
	unsigned long UBRR_value = F_CPU/16/baud_rate-1;

	UCSRA = 0x0;
	UCSRB = _BV(RXEN) | _BV(TXEN);   // Enable RX and TX pins
	UCSRC = _BV(USBS) | _BV(UCSZ0) | _BV(UCSZ1); // 8-bit data and 2 stop bits

	// Not gonna trust the compiler and use UBBR
	UBRRH = (unsigned char)(UBRR_value >> 8); // Set the H register
	UBRRL = (unsigned char)UBRR_value; // Set the L register
}

void uart_putchar(char c)
{
  loop_until_bit_is_set(UCSRA, UDRE); /* Wait until data register empty. */
  UDR = c;
}

void uart_putchar2(char c)
{
  UDR = c;
  loop_until_bit_is_set(UCSRA, TXC); /* Wait until transmission ready. */
}

void uart_putchar_safe(char c)
{
  loop_until_bit_is_set(UCSRA, UDRE); /* Wait until data register empty. */
  UDR = c;
  loop_until_bit_is_set(UCSRA, TXC); /* Wait until transmission ready. */
}

char uart_getchar(void) {
  // loop_until_bit_is_set(UCSRA, UDRE); /* Don't read what you wrote */
  loop_until_bit_is_set(UCSRA, RXC); /* Wait until data exists. */
  return UDR;
}

char uart_getchar_timed(bool* not_timed_out, unsigned long timeout)
{
	unsigned long start_time = millis();
	while ( !( UCSRA & (1 << UDRE) ) ) {
		if (millis() < start_time + timeout) {
			*not_timed_out = false;
			return '\0';
    }
  }
	*not_timed_out = true;
  return UDR;
}

bool uart_find(const char* target, unsigned long timeout)
{
	unsigned int i = 0;
	bool not_timed_out = true;
	while (target[i++] && not_timed_out)
  	if (uart_getchar_timed(&not_timed_out, timeout) != target[i])
			i = 0;
	return not_timed_out;
}

void uart_wait_for(const char* target)
{
  unsigned int i = 0;
  while (target[i++])
    if (uart_getchar() != target[i])
      i = 0;
}

int uart_parseInt(unsigned long timeout)
{
	bool not_timed_out = true;
	long value = 0;
	char c;

	c = uart_getchar_timed(&not_timed_out, timeout);
	if (!not_timed_out)
		return 0;
	bool isNegative = (c == '-');

	do {
		value = value * 10 + c - '0';
    	c = uart_getchar_timed(&not_timed_out, timeout);
	}
	while( (c >= '0' && c <= '9') && not_timed_out );
	return value;
}

bool uart_readBytes(char* buffer, unsigned int len, unsigned long timeout)
{
	unsigned int i = 0;
	bool not_timed_out = true;
	while (i < len && not_timed_out)
      buffer[i++] = uart_getchar_timed(&not_timed_out, timeout);
	return not_timed_out;
}

void uart_memcpy(unsigned char* dest, unsigned int len)
{
	while (len--)
		*dest = uart_getchar();
}
