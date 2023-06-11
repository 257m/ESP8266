extern "C" {
	/// Proprietary SDK includes
	#include "ets_sys.h"
	#include "espconn.h"
	#include "os_type.h"
	#include "osapi.h"
	#include "mem.h"
	#include "user_interface.h"
	#include "espnow.h"

	/// Custom libraries for uart_init, printf, gets, etc...	
	#include "uart_io.h"
	#include "serial_io.h"
	#include "web_server.h"
}

unsigned char server_mac [] = {0x2e,0xf4,0x32,0x4c,0xbe,0x11};

typedef struct {
	int x, y;
} Message;

void on_data_recv(uint8_t* mac, uint8_t* data, unsigned char len)
{
	Message* m = (Message*)data;
	printf("MAC_ADDR: %x:%x:%x:%x\r\n", mac[0], mac[1], mac[2], mac[3]);
	printf("Length: %d\r\n", len);
	printf("DATA:\r\n");
	for (unsigned int i = 0;i < len;i++)
		printf("%d:\t%d\r\n", i, data[i]);
}

void setup()
{
	uart_init(9600);
	//serial_write("Uart is functional\r\n");
	//wifi_set_macaddr(AP_IF, server_mac);
	web_server_init("EspWebServer", "PASSWORD", 6, false);
	if (esp_now_init())
		printf("Error initializing ESP-NOW");
	esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
	esp_now_register_recv_cb(on_data_recv);
}

void loop()
{

}
