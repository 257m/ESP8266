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

void uart_putchar(unsigned char c)
{
	Serial.write(c);
}

void uart_memcpy(unsigned char* dest, unsigned int len)
{
	while (!Serial.available());
	Serial.readBytes(dest, len); 
}

void mem_cpy(unsigned char* dest, unsigned char* source, unsigned int len)
{
	while (len--)
		*dest++ = *source++;
}

unsigned char server_mac [] = {0x2e,0xf4,0x32,0x4c,0xbe,0x11};

// Structure for message
typedef struct {
	int x, y;
} Message;

// Will run when data is received
void on_data_recv(uint8_t* mac, uint8_t* data, unsigned char len)
{
	#if DEBUG
	// Print out data received
	Message m;
	// Casts because -fpermissive
	mem_cpy((unsigned char*)&m, (unsigned char*)data, len);
	printf("MAC_ADDR: %x:%x:%x:%x\r\n", mac[0], mac[1], mac[2], mac[3]);
	printf("Length: %d\r\n", len);
	printf("DATA: %d, %d\r\n", m.x, m.y);
	#else
	// Send zero through uart to tell atmega we are sending joystick data
	uart_putchar(0);
	serial_write_count((char*)data, len);
	#endif /* DEBUG */
}

void setup()
{
	// Initialize uart at 9600 baud
	Serial.begin(9600);
	//wifi_set_macaddr(SOFTAP_IF, server_mac);
	// Initialize web server with ssid EspWebServ
	web_server_init("EspWebServer", "PASSWORD", 6, false);
#if DEBUG
	wifi_get_macaddr(SOFTAP_IF, server_mac);
	printf("%d, %d, %d, %d\r\n", server_mac[0], server_mac[1], server_mac[2], server_mac[3]);
#endif
	// Initialize esp now
	if (esp_now_init()) {
		PRINTF("Error initializing ESP-NOW");
	}
	// Set role as slave (joystick esp is the controller)
	esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
	// Set the callback function
	esp_now_register_recv_cb(on_data_recv);
}

void loop()
{

}
