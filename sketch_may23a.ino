extern "C" {
  #include "serial_io.h"
  #include "osapi.h"
}

void setup()
{
  uart_init(115200);
}

void loop()
{
  os_printf("hello, world\n");
  printf("hello, world\n");
}