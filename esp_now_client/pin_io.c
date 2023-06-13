#include "pin_io.h"

inline void set_pin(unsigned char pin, bool state)
{
	GPIO_REG_WRITE(GPIO_OUT_W1TC - (0x4*state), pin); 		
}

inline void read_pin(unsigned char pin)
{
	return ((*((volatile unsigned int*)GPIO_IN)) >> pin) & 0x1;
}

inline void set_pin_mode(unsigned char pin, bool state)
{
  GPIO_REG_WRITE(GPIO_ENABLE_W1TC - (0x4*state), pin);  
}
