// Include my libraries
extern "C" {
  #include "serial_io.h"
  #include "pin_io.h"
}

void setup()
{
	//uart_select(2);
	uart_init(9600);
	serial_write("Uart is functional\r\n");
}

void loop()
{
	serial_write("hello, world\n");
}
