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
}

typedef struct {
	int x, y;
} Message;
// Actual MAC of mega 0x2e,0xf4,0x32,0x4c,0xbe,0x11 
unsigned char server_mac [] = {0x2e,0xf4,0x32,0x4c,0xbe,0x11};

void on_data_sent(unsigned char *mac_addr, unsigned char status)
{
	printf("Sent with status: %d\r\n", status);
}

void setup() {
	uart_init(9600);
	wifi_set_opmode(STATION_MODE);
	if (esp_now_init())
		printf("ERROR with esp_now_init\r\n");
	esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
	esp_now_register_send_cb(on_data_sent);
	if (esp_now_add_peer(server_mac, ESP_NOW_ROLE_SLAVE, 0, NULL, 16))
		printf("Failed to add peer\n");
}

void loop() {
	Message m;
	digitalWrite(0, HIGH);
	digitalWrite(1, LOW);
	m.x = analogRead(A0);
	digitalWrite(0, LOW);
	digitalWrite(1, HIGH);
	m.y = analogRead(A0);
	if (esp_now_send(server_mac, (unsigned char*)&m, sizeof(Message)))
		printf("SUCCESS\r\n");
	else
		printf("FAILED\r\n");
	delay(3000);
}
