#ifndef UART_IO_H
#define UART_IO_H

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h> 

#define F_CPU 16000000UL

#ifdef loop_until_bit_is_set
#undef loop_until_bit_is_set
#endif
#define loop_until_bit_is_set(R, F) while ( !( R & (1 << F)) );
#define loop_until_bit_isnt_set(R, F) while ( R & (1 << F) );

#define _BV(bit) (1 << (bit))

#ifndef F_CPU
#warning "F_CPU is not defined"
#endif

extern inline bool uart_available();

extern inline void uart_select(unsigned char selected);

extern void uart_init(unsigned long baud_rate);

extern void uart_putchar(char c);

extern void uart_putchar2(char c);

extern void uart_putchar_safe(char c);

extern char uart_getchar(void);

extern char uart_getchar_timed(bool* not_timed_out, unsigned long timeout);

extern bool uart_find(const char* target, unsigned long timeout);

extern void uart_wait_for(const char* target);

extern int uart_parseInt(unsigned long timeout);

extern bool uart_readBytes(char* buffer, unsigned int len, unsigned long timeout);

extern inline void uart_memcpy(unsigned char* dest, unsigned int len);
#endif /* UART_IO_H */
