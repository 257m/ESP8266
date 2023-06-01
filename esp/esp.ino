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

void setup()
{
	uart_init(9600);
	// serial_write("Uart is functional\r\n");
	web_server_init("SSID", "PASSWORD", 6, false);
}

unsigned long last_time = 0;
const char station_names [][24] = {
  "STATION_IDLE",
  "STATION_CONNECTING",
  "STATION_WRONG_PASSWORD",
  "STATION_NO_AP_FOUND",
  "STATION_CONNECT_FAIL",
  "STATION_GOT_IP",
};

void loop()
{
   if ((millis() - last_time) < 10000)
    return;
   printf("Waiting for ip... Current Status = %s\r\n", station_names[wifi_station_get_connect_status()]);
   last_time = millis();
}
