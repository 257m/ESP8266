#ifndef PIN_IO_H
#define PIN_IO_H

#include <stdbool.h>
#include <avr/io.h>

#define set_port_mode(port, num) DDR##port = num
#define set_port(port, num) PORT##D = num

#ifndef LOW
#define LOW 0
#endif

#ifndef HIGH
#define HIGH 1
#endif

#ifndef INPUT
#define INPUT 0
#endif

#ifndef OUTPUT
#define OUTPUT 1
#endif

extern inline void set_port_pin_low(volatile unsigned char* port, unsigned char pin);

extern inline void set_port_pin_high(volatile unsigned char* port, unsigned char pin);

extern void (*set_port_pin_array[2])(volatile unsigned char*, unsigned char);

#define A0   (54)
#define A1   (55)
#define A2   (56)
#define A3   (57)
#define A4   (58)
#define A5   (59)
#define A6   (60)
#define A7   (61)
#define A8   (62)
#define A9   (63)
#define A10  (64)
#define A11  (65)
#define A12  (66)
#define A13  (67)
#define A14  (68)
#define A15  (69)

extern inline void set_pin(unsigned char pin, bool state);

extern inline void set_pin_mode(unsigned char pin, bool state);

extern inline bool read_pin(unsigned char pin);

#endif /* PIN_IO_H */