#include "pin_io.h"

inline void set_port_pin_low(volatile unsigned char* port, unsigned char pin)
{
	*port &= pin;
}

inline void set_port_pin_high(volatile unsigned char* port, unsigned char pin)
{
	*port |= pin;
}

void (*set_port_pin_array[2])(volatile unsigned char*, unsigned char) = {
  &set_port_pin_low,
  &set_port_pin_high,
};

/*
  Can't believe I'm doing this.
  This is feels wrong in every way
  but I can't think of a better way and
  apparently neither has anybody else.
  Blame the very weird pin layout
*/

const volatile unsigned char* pin_to_port_base[] = {
	// PORTLIST		
	// -------------------------------------------		
	&PINE	, // PE 0 ** 0 ** USART0_RX	
	&PINE	, // PE 1 ** 1 ** USART0_TX	
	&PINE	, // PE 4 ** 2 ** PWM2	
	&PINE	, // PE 5 ** 3 ** PWM3	
	&PING	, // PG 5 ** 4 ** PWM4	
	&PINE	, // PE 3 ** 5 ** PWM5	
	&PINH	, // PH 3 ** 6 ** PWM6	
	&PINH	, // PH 4 ** 7 ** PWM7	
	&PINH	, // PH 5 ** 8 ** PWM8	
	&PINH	, // PH 6 ** 9 ** PWM9	
	&PINB	, // PB 4 ** 10 ** PWM10	
	&PINB	, // PB 5 ** 11 ** PWM11	
	&PINB	, // PB 6 ** 12 ** PWM12	
	&PINB	, // PB 7 ** 13 ** PWM13	
	&PINJ	, // PJ 1 ** 14 ** USART3_TX	
	&PINJ	, // PJ 0 ** 15 ** USART3_RX	
	&PINH	, // PH 1 ** 16 ** USART2_TX	
	&PINH	, // PH 0 ** 17 ** USART2_RX	
	&PIND	, // PD 3 ** 18 ** USART1_TX	
	&PIND	, // PD 2 ** 19 ** USART1_RX	
	&PIND	, // PD 1 ** 20 ** I2C_SDA	
	&PIND	, // PD 0 ** 21 ** I2C_SCL	
	&PINA	, // PA 0 ** 22 ** D22	
	&PINA	, // PA 1 ** 23 ** D23	
	&PINA	, // PA 2 ** 24 ** D24	
	&PINA	, // PA 3 ** 25 ** D25	
	&PINA	, // PA 4 ** 26 ** D26	
	&PINA	, // PA 5 ** 27 ** D27	
	&PINA	, // PA 6 ** 28 ** D28	
	&PINA	, // PA 7 ** 29 ** D29	
	&PINC	, // PC 7 ** 30 ** D30	
	&PINC	, // PC 6 ** 31 ** D31	
	&PINC	, // PC 5 ** 32 ** D32	
	&PINC	, // PC 4 ** 33 ** D33	
	&PINC	, // PC 3 ** 34 ** D34	
	&PINC	, // PC 2 ** 35 ** D35	
	&PINC	, // PC 1 ** 36 ** D36	
	&PINC	, // PC 0 ** 37 ** D37	
	&PIND	, // PD 7 ** 38 ** D38	
	&PING	, // PG 2 ** 39 ** D39	
	&PING	, // PG 1 ** 40 ** D40	
	&PING	, // PG 0 ** 41 ** D41	
	&PINL	, // PL 7 ** 42 ** D42	
	&PINL	, // PL 6 ** 43 ** D43	
	&PINL	, // PL 5 ** 44 ** D44	
	&PINL	, // PL 4 ** 45 ** D45	
	&PINL	, // PL 3 ** 46 ** D46	
	&PINL	, // PL 2 ** 47 ** D47	
	&PINL	, // PL 1 ** 48 ** D48	
	&PINL	, // PL 0 ** 49 ** D49	
	&PINB	, // PB 3 ** 50 ** SPI_MISO	
	&PINB	, // PB 2 ** 51 ** SPI_MOSI	
	&PINB	, // PB 1 ** 52 ** SPI_SCK	
	&PINB	, // PB 0 ** 53 ** SPI_SS	
	&PINF	, // PF 0 ** 54 ** A0	
	&PINF	, // PF 1 ** 55 ** A1	
	&PINF	, // PF 2 ** 56 ** A2	
	&PINF	, // PF 3 ** 57 ** A3	
	&PINF	, // PF 4 ** 58 ** A4	
	&PINF	, // PF 5 ** 59 ** A5	
	&PINF	, // PF 6 ** 60 ** A6	
	&PINF	, // PF 7 ** 61 ** A7	
	&PINK	, // PK 0 ** 62 ** A8	
	&PINK	, // PK 1 ** 63 ** A9	
	&PINK	, // PK 2 ** 64 ** A10	
	&PINK	, // PK 3 ** 65 ** A11	
	&PINK	, // PK 4 ** 66 ** A12	
	&PINK	, // PK 5 ** 67 ** A13	
	&PINK	, // PK 6 ** 68 ** A14	
	&PINK	, // PK 7 ** 69 ** A15	
};

#define pin_to_input_port(n) pin_to_port_base[n]
#define pin_to_output_port(n) (pin_to_port_base[n] + 0x2)
#define pin_to_mode_port(n) (pin_to_port_base[n] + 0x1)

const unsigned char pin_to_mask[] = {
	// PIN IN PORT		
	// -------------------------------------------		
	_BV( 0 )	, // PE 0 ** 0 ** USART0_RX	
	_BV( 1 )	, // PE 1 ** 1 ** USART0_TX	
	_BV( 4 )	, // PE 4 ** 2 ** PWM2	
	_BV( 5 )	, // PE 5 ** 3 ** PWM3	
	_BV( 5 )	, // PG 5 ** 4 ** PWM4	
	_BV( 3 )	, // PE 3 ** 5 ** PWM5	
	_BV( 3 )	, // PH 3 ** 6 ** PWM6	
	_BV( 4 )	, // PH 4 ** 7 ** PWM7	
	_BV( 5 )	, // PH 5 ** 8 ** PWM8	
	_BV( 6 )	, // PH 6 ** 9 ** PWM9	
	_BV( 4 )	, // PB 4 ** 10 ** PWM10	
	_BV( 5 )	, // PB 5 ** 11 ** PWM11	
	_BV( 6 )	, // PB 6 ** 12 ** PWM12	
	_BV( 7 )	, // PB 7 ** 13 ** PWM13	
	_BV( 1 )	, // PJ 1 ** 14 ** USART3_TX	
	_BV( 0 )	, // PJ 0 ** 15 ** USART3_RX	
	_BV( 1 )	, // PH 1 ** 16 ** USART2_TX	
	_BV( 0 )	, // PH 0 ** 17 ** USART2_RX	
	_BV( 3 )	, // PD 3 ** 18 ** USART1_TX	
	_BV( 2 )	, // PD 2 ** 19 ** USART1_RX	
	_BV( 1 )	, // PD 1 ** 20 ** I2C_SDA	
	_BV( 0 )	, // PD 0 ** 21 ** I2C_SCL	
	_BV( 0 )	, // PA 0 ** 22 ** D22	
	_BV( 1 )	, // PA 1 ** 23 ** D23	
	_BV( 2 )	, // PA 2 ** 24 ** D24	
	_BV( 3 )	, // PA 3 ** 25 ** D25	
	_BV( 4 )	, // PA 4 ** 26 ** D26	
	_BV( 5 )	, // PA 5 ** 27 ** D27	
	_BV( 6 )	, // PA 6 ** 28 ** D28	
	_BV( 7 )	, // PA 7 ** 29 ** D29	
	_BV( 7 )	, // PC 7 ** 30 ** D30	
	_BV( 6 )	, // PC 6 ** 31 ** D31	
	_BV( 5 )	, // PC 5 ** 32 ** D32	
	_BV( 4 )	, // PC 4 ** 33 ** D33	
	_BV( 3 )	, // PC 3 ** 34 ** D34	
	_BV( 2 )	, // PC 2 ** 35 ** D35	
	_BV( 1 )	, // PC 1 ** 36 ** D36	
	_BV( 0 )	, // PC 0 ** 37 ** D37	
	_BV( 7 )	, // PD 7 ** 38 ** D38	
	_BV( 2 )	, // PG 2 ** 39 ** D39	
	_BV( 1 )	, // PG 1 ** 40 ** D40	
	_BV( 0 )	, // PG 0 ** 41 ** D41	
	_BV( 7 )	, // PL 7 ** 42 ** D42	
	_BV( 6 )	, // PL 6 ** 43 ** D43	
	_BV( 5 )	, // PL 5 ** 44 ** D44	
	_BV( 4 )	, // PL 4 ** 45 ** D45	
	_BV( 3 )	, // PL 3 ** 46 ** D46	
	_BV( 2 )	, // PL 2 ** 47 ** D47	
	_BV( 1 )	, // PL 1 ** 48 ** D48	
	_BV( 0 )	, // PL 0 ** 49 ** D49	
	_BV( 3 )	, // PB 3 ** 50 ** SPI_MISO	
	_BV( 2 )	, // PB 2 ** 51 ** SPI_MOSI	
	_BV( 1 )	, // PB 1 ** 52 ** SPI_SCK	
	_BV( 0 )	, // PB 0 ** 53 ** SPI_SS	
	_BV( 0 )	, // PF 0 ** 54 ** A0	
	_BV( 1 )	, // PF 1 ** 55 ** A1	
	_BV( 2 )	, // PF 2 ** 56 ** A2	
	_BV( 3 )	, // PF 3 ** 57 ** A3	
	_BV( 4 )	, // PF 4 ** 58 ** A4	
	_BV( 5 )	, // PF 5 ** 59 ** A5	
	_BV( 6 )	, // PF 6 ** 60 ** A6	
	_BV( 7 )	, // PF 7 ** 61 ** A7	
	_BV( 0 )	, // PK 0 ** 62 ** A8	
	_BV( 1 )	, // PK 1 ** 63 ** A9	
	_BV( 2 )	, // PK 2 ** 64 ** A10	
	_BV( 3 )	, // PK 3 ** 65 ** A11	
	_BV( 4 )	, // PK 4 ** 66 ** A12	
	_BV( 5 )	, // PK 5 ** 67 ** A13	
	_BV( 6 )	, // PK 6 ** 68 ** A14	
	_BV( 7 )	, // PK 7 ** 69 ** A15	
};

inline void set_pin(unsigned char pin, bool state)
{
 	set_port_pin_array[state](pin_to_output_port(pin), pin_to_mask[pin]);
}

inline void set_pin_mode(unsigned char pin, bool state)
{
	set_port_pin_array[state](pin_to_mode_port(pin), pin_to_mask[pin]);
}

inline bool read_pin(unsigned char pin)
{
  // Hopefully actually converts to 1 or 0
  // and not just write all of it into the A register
  // but if it does it is not the biggest deal
  // because most of the time it won't be compared
  // with TRUE/FALSE directly
	return (*pin_to_input_port(pin) & pin_to_mask[pin]);
}