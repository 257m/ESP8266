#ifndef STRING_H
#define STRING_H

extern unsigned int str_len(char* str)
extern void str_cpy(char* dest, char* source, unsigned int size)
extern char* str_find_char(char* str, char to_find)
extern char* str_dup(char* str)

#endif /* STRING_H */
