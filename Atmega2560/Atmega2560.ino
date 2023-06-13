// Include my libraries
extern "C" {
  #include "serial_io.h"
  #include "pin_io.h"
}

void setup()
{
	Serial.begin(9600);
	serial_write("Uart is functional\r\n");
}

void loop()
{
	serial_write("hello, world\n");
}
