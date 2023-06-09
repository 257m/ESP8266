extern "C" {
	/// Proprietary SDK includes
	#include "ets_sys.h"
	#include "espconn.h"
	#include "os_type.h"
	#include "osapi.h"
	#include "mem.h"
	#include "user_interface.h"

	/// Custom libraries for uart_init, printf, gets, etc...	
	#include "uart_io.h"
	#include "serial_io.h"
	#include "web_client.h"
}

void setup()
{
	uart_init(9600);
	//serial_write("Uart is functional\r\n");
	web_client_init("WRDSB Wireless", "", 6, false);
}

static void onreceive(Request* req)
{
  printf("Received\r\n");
}

void loop()
{
  web_client_get("google.com", "", onreceive);
}
