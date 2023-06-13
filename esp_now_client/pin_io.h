#ifndef PIN_IO_H
#define PIN_IO_H

#include <stdbool.h>

#define LOW 0
#define HIGH 1

#define INPUT 0
#define OUTPUT 1

#define PERIPHS_GPIO_BASEADDR 0x60000300
#define GPIO_OUT_W1TS 0x60000304
#define GPIO_OUT_W1TC 0x60000308
#define GPIO_IN 0x60000318

#defineS GPIO_REG_WRITE(addr, val) (*((volatile unsigned int*)addr)) = (1 << val)

extern inline void set_pin(unsigned char pin, bool state);
extern inline void read_pin(unsigned char pin);

#endif /* PIN_IO_H */
