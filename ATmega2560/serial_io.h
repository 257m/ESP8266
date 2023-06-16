#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#include <stdarg.h>
#include <stddef.h>
#include "uart_io.h"

extern void serial_write(char* buffer);

extern void serial_write_count(char* buffer, unsigned int len);

extern unsigned int bstrnatt(char* buffer, unsigned int buffer_size, unsigned int bi, char* str);

extern inline unsigned int count_digits(int64_t n, uint8_t base);

extern inline unsigned int ucount_digits(uint64_t n, uint8_t base);

extern unsigned int binttostr(char* buffer, unsigned int buffer_size, unsigned int bi, int n, uint8_t base);

extern unsigned int buinttostr(char* buffer, unsigned int buffer_size, unsigned int bi, unsigned int n, uint8_t base);

extern unsigned int blinttostr(char* buffer, unsigned int buffer_size, unsigned int bi, long n, uint8_t base);

extern unsigned int bluinttostr(char* buffer, unsigned int buffer_size, unsigned int bi, unsigned long n, uint8_t base);

extern double rudimentary_pow10(unsigned int n);

extern unsigned int bfloattostrdec(char* buffer, unsigned int buffer_size, unsigned int bi, unsigned int decimals, double n);

extern unsigned int str_len(char* str);

extern char* str_find_char(char* str, unsigned int size, char to_find);

#ifdef gets
#undef gets
#endif
#define gets _gets

extern void gets(char* str, unsigned int size);

#ifdef vsnprintf
#undef vsnprintf
#endif
#define vsnprintf _vsnprintf

extern unsigned int vsnprintf(char* buffer, unsigned int buffer_size, const char* format, va_list args);

#ifdef printf
#undef printf
#endif
#define printf _printf

extern unsigned int printf(const char *format, ...);

extern char* aprintf(const char *format, ...);

#endif /* SERIAL_IO_H */
