#ifndef SERIAL_IO_H
#define SERIAL_IO_H

/// STDLIB
#include <stdarg.h>
#include <stddef.h>

/// Proprietary SDK
#include "lwip/mem.h"

#include "uart_io.h"

extern void serial_write(char* buffer);

extern unsigned int serial_write_count(char* buffer);

extern unsigned int bstrnatt(char* buffer, unsigned int buffer_size, unsigned int bi, char* str);

extern inline unsigned int count_digits(long n, unsigned char base);

extern inline unsigned int ucount_digits(unsigned long n, unsigned char base);

extern unsigned int binttostr(char* buffer, unsigned int buffer_size, unsigned int bi, int n, unsigned char base);

extern unsigned int buinttostr(char* buffer, unsigned int buffer_size, unsigned int bi, unsigned int n, unsigned char base);

extern unsigned int blinttostr(char* buffer, unsigned int buffer_size, unsigned int bi, long n, unsigned char base);

extern unsigned int bluinttostr(char* buffer, unsigned int buffer_size, unsigned int bi, unsigned long n, unsigned char base);

extern double rudimentary_pow10(unsigned int n);

extern unsigned int bfloattostrdec(char* buffer, unsigned int buffer_size, unsigned int bi, unsigned int decimals, double n);

extern unsigned int str_len(char* str);

extern char* str_find_char(char* str, char to_find);

#undef gets
#define gets _gets

extern void gets(char* str, unsigned int size);

#undef vsnprintf
#define vsnprintf _vsnprintf

extern unsigned int vsnprintf(char* buffer, unsigned int buffer_size, const char* format, va_list args);

#undef printf
#define printf _printf

extern unsigned int printf(const char *format, ...);

extern char* aprintf(const char *format, ...);

#undef snprintf
#define snprintf _snprintf

unsigned int snprintf(char* buffer, unsigned int buffer_size, const char* format, ...);

#endif /* SERIAL_IO_H */
