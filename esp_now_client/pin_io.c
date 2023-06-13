#include "pin_io.h"

inline void set_pin(unsigned char pin, bool state)
{
	GPIO_REG_WRITE(GPIO_OUT_W1TS + (0x4*state), pin); 		
}

inline void read_pin(unsigned char pin)
{
	return ((*((volatile unsigned int*)GPIO_IN)) >> pin) & 0x1;
}
