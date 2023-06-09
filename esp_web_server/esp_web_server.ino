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
	#include "web_server.h"
}

typedef struct {
	int x, y;
} Message;

void on_data_recv(const uint8_t* mac, Message* incomingData, int len)
{
	printf("%d, %d\n", x, y);
}

void setup()
{
	uart_init(9600);
	//serial_write("Uart is functional\r\n");
	web_server_init("EspWebServer", "PASSWORD", 6, false);
	if (esp_now_init()) {
		printf("Error initializing ESP-NOW");

	esp_now_register_recv_cb(on_data_recv);	
}

void loop()
{

}
