#include "string.h"
#include "osapi.h"

unsigned int str_len(char* str)
{
	unsigned int str_count = 0;
	while (*str) {
		str_count++;
		str++;
	}
}

void str_cpy(char* dest, char* source, unsigned int size)
{
	while (dest < (dest + size) && *source)
		*dest++ = *source++;
	*dest = '\0';
}

char* str_find_char(char* str, char to_find)
{
	while (*str) {
		if (*str == to_find)
			return str;
		str++;
	}
	return NULL; 
}

char* str_dup(char* str)
{
	char* new_str = malloc(str_len(str));
	char* str_itr = new_str;
	while (*str)
		*str_itr++ = *str++;
	return new_str;
}

int atoi(char* str)
{
	int total = 0;
	char isnegative = 1;
	while ((*str >= '0' && *str <= '9') || *str == '-') {
		if (*str == '-')
			isnegative = -1;
		total *= 10;
		total += *str++;
	}
	total *= isnegative;
	return total;
}
