#include "serial_io.h"

void serial_write(char* buffer)
{
	while (*buffer)
		uart_putchar(*(buffer++));
}

unsigned int serial_write_count(char* buffer)
{
	unsigned int i = 0;
	while (buffer[i] != '\0')
		uart_putchar(buffer[i++]);
	return i;
}

unsigned int bstrnatt
(char* buffer, unsigned int buffer_size,
 unsigned int bi, char* str)
{
	unsigned int i = 0;
	while (bi < buffer_size && str[i] != '\0')
		buffer[bi++] = str[i++];
	while (str[i] != '\0')
		i++;
	return i;
}

inline unsigned int count_digits(long n, unsigned char base)
{
	unsigned int digits = 1;
	while (n /= base)
		digits++;
	return digits;
}

inline unsigned int ucount_digits(unsigned long n, unsigned char base)
{
	unsigned int digits = 1;
	while (n /= base)
		digits++;
	return digits;
}

// supports arbitary base but only base 10 will be used in the custom printf
unsigned int binttostr
(char* buffer, unsigned int buffer_size,
 unsigned int bi, int n, unsigned char base)
{
	unsigned int digits = count_digits(n, base) + (n < 0);
	if (bi+digits >= buffer_size)
		return digits;
	char* str = &buffer[bi+digits];

	if (n < 0) {
		buffer[bi] = '-';
		n *= -1;
	}

	do {
		char c = n % base;
		n /= base;
		*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while(n && (bi < buffer_size));
	return digits;
}

unsigned int buinttostr
(char* buffer, unsigned int buffer_size,
 unsigned int bi, unsigned int n, unsigned char base)
{
	unsigned int digits = ucount_digits(n, base);
	if (bi+digits >= buffer_size)
		return digits;
	char *str = &buffer[bi+digits];

	do {
		char c = n % base;
		n /= base;
		*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while(n && bi < buffer_size);
	return digits;
}

unsigned int blinttostr
(char* buffer, unsigned int buffer_size,
 unsigned int bi, long n, unsigned char base)
{
	unsigned int digits = count_digits(n, base) + (n < 0);
	if (bi+digits >= buffer_size)
		return digits;
	char *str = &buffer[bi+digits];

	if (n < 0) {
		buffer[bi] = '-';
		n *= -1;
	}

	do {
		char c = n % base;
		n /= base;
		*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while(n && bi < buffer_size);
	return digits;
}

unsigned int bluinttostr
(char* buffer, unsigned int buffer_size,
 unsigned int bi, unsigned long n, unsigned char base)
{
	unsigned int digits = ucount_digits(n, base);
	if (bi+digits >= buffer_size)
		return digits;
	char *str = &buffer[bi+digits];

	do {
		char c = n % base;
		n /= base;
		*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while(n && bi < buffer_size);
	return digits;
}

inline double rudimentary_pow10(unsigned int n)
{
	double ans = 1;
	while((n--))
		ans *= 10;
	return ans;
}

unsigned int bfloattostrdec
(char* buffer, unsigned int buffer_size,
 unsigned int bi, unsigned int decimals, double n)
{
	long long int_part = (long long) n;
	double float_part = n - ((double)int_part);
	unsigned int bi_save = bi;
	bi += binttostr(buffer, buffer_size, bi, int_part, 10);
	if (decimals) {
		if (bi < buffer_size)
			buffer[bi] = '.';
		bi++;
		bi += buinttostr(buffer, buffer_size, bi, (unsigned int)(float_part * rudimentary_pow10(decimals)), 10);
	}
	return bi - bi_save;
}

void gets(char* str, unsigned int size)
{
	const char* last = str + size;
	while (str < last) {
		if ((*str = uart_getchar()) == '\n')
			return;
		str++;
	}
}

unsigned int vsnprintf
(char* buffer, unsigned int buffer_size,
 const char* format, va_list args)
{
	unsigned int bi = 0;
	unsigned int fi = 0;
	while (format[fi]) {
		if (format[fi] == '%') {
			switch (format[++fi]) {
				case 's':
					bi += bstrnatt(buffer, buffer_size, bi, va_arg(args, char*));
					break;
				case 'c':
					if (bi < buffer_size)
						buffer[bi] = va_arg(args, char);
					bi++;
					break;
				case 'd':
				case 'i':
					bi += binttostr(buffer, buffer_size, bi, va_arg(args, int), 10);
					break;
				case 'u':
					bi += buinttostr(buffer, buffer_size, bi, va_arg(args, unsigned int), 10);
					break;
				case 'f':
					bi += bfloattostrdec(buffer, buffer_size, bi, 3, va_arg(args, double));
					break;
				case 'l':
					switch(format[++fi]) {
						case 'd':
						case 'i':
							bi += blinttostr(buffer, buffer_size, bi, va_arg(args,long int), 10);
							break;
						case 'u':
							bi += bluinttostr(buffer, buffer_size, bi, va_arg(args, long unsigned int), 10);
							break;
					}
					break;
				case 'x':
					bi += buinttostr(buffer, buffer_size, bi, va_arg(args, unsigned int), 6);
					break;
				case 'b':
					bi += buinttostr(buffer, buffer_size, bi, va_arg(args, unsigned int), 2);
					break;
				default:
					break;
			}
			fi++;
		}
		else {
			if (bi < buffer_size)
				buffer[bi] = format[fi];
			bi++;
			fi++;
		}
	}
	if (bi < buffer_size)
		buffer[bi] = '\0';
	return bi;
}

unsigned int printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	char temp[64] = "";
	char* buffer = temp;
	unsigned int len = vsnprintf(temp, sizeof(temp), format, args);
	va_end(args);
	if (len > sizeof(temp) - 1) {
		char* buffer = (char*)malloc(len + 1);
		if (!buffer)
			return 0;
		va_start(args, format);
		len = vsnprintf(buffer, len + 1, format, args);
		va_end(args);
	}
	serial_write(buffer);
	if (buffer != temp)
		free(buffer);
	return len;
}

char* aprintf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	unsigned int len = vsnprintf(NULL, 0, format, args);
	va_end(args);
	char* buffer = (char*)malloc(len + 1);
	if (!buffer)
		return NULL;
	va_start(args, format);
	vsnprintf(buffer, len + 1, format, args);
	va_end(args);

	return buffer;
}

unsigned int snprintf(char* buffer, unsigned int buffer_size, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	unsigned int len = vsnprintf(buffer, buffer_size, format, args);
	va_end(args);
	return len;  
}
