extern "C" {
	/// Proprietary SDK includes
	#include "ets_sys.h"
	#include "espconn.h"
	#include "os_type.h"
	#include "osapi.h"
	#include "mem.h"
	#include "user_interface.h"
	#include "esp_now.h"

	/// Custom libraries for uart_init, printf, gets, etc...	
	#include "uart_io.h"
	#include "serial_io.h"
	#include "web_server.h"
}

typedef struct {
	int x, y;
} Message;

const uint8_t server_mac [] = {0x8c,0xce,0x4e,0xce,0x6a,0x14};

esp_now_peer_info_t peer_info = {
	.peer_addr = {0x8c,0xce,0x4e,0xce,0x6a,0x14},
	.channel = 0,
	.encrypt = 0,
};

void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
	
}

void setup() {
	wifi_set_opmode(STATION_MODE);
	if (esp_now_init())
		printf("ERROR with esp_now_init\r\n");
	esp_now_register_send_cb(on_data_sent);
	if (esp_now_add_peer(&peer_info))
		printf("Failed to add peer\n");
}

void loop() {
	Message m = {10, 10};
	if (esp_now_send(server_mac, &m, sizeof(Message)))
		printf("SUCCESS\r\n");
	else
		printf("FAILED\r\n");
	delay(3000);
}
