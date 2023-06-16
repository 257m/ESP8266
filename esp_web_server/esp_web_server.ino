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

void mem_cpy(unsigned char* dest, unsigned char* source, unsigned int len)
{
	while (len--)
		*dest++ = *source++;
}

unsigned char server_mac [] = {0x2e,0xf4,0x32,0x4c,0xbe,0x11};

typedef struct {
	int x, y;
} Message;

void on_data_recv(uint8_t* mac, uint8_t* data, unsigned char len)
{
	#if DEBUG
	Message m;
	// Casts because -fpermissive
	mem_cpy((unsigned char*)&m, (unsigned char*)data, len);
	printf("MAC_ADDR: %x:%x:%x:%x\r\n", mac[0], mac[1], mac[2], mac[3]);
	printf("Length: %d\r\n", len);
	printf("DATA: %d, %d\r\n", m.x, m.y);
	#else
	uart_putchar(0);
	serial_write_count((char*)&data, len);
	#endif /* DEBUG */
}

void setup()
{
	uart_init(9600);
	web_server_init("EspWebServer", "PASSWORD", 6, false);
	if (esp_now_init()) {
		PRINTF("Error initializing ESP-NOW");
	}
	esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
	esp_now_register_recv_cb(on_data_recv);
}

void loop()
{

}
