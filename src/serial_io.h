#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#include <stdarg.h>
#include <stddef.h>
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

#ifdef INCLUDING_ARDUINO | gets
  #undef gets
  #define gets _gets
#endif

extern void gets(char* str, unsigned int size);

#ifdef INCLUDING_ARDUINO | vsnprintf
  #undef vsnprintf
  #define vsnprintf _vsnprintf
#endif

extern unsigned int vsnprintf(char* buffer, unsigned int buffer_size, const char* format, va_list args);

#ifdef INCLUDING_ARDUINO | printf
  #undef printf
  #define printf _printf
#endif

extern unsigned int printf(const char *format, ...);

extern char* aprintf(const char *format, ...);

#endif /* SERIAL_IO_H */