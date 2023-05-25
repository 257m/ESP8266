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

void web_server_init()
{
	struct station_config stconfig = {
		.ssid = SSID,
		.password = PASSWORD,
	};
	stconfig.threshold.authmode = AUTH_WPA_WPA2_PSK;
	stconfig.threshold.rssi = 84;

	printf("connecting to %s with %s\r\nrssi: %d auth: %d\r\n", stconfig.ssid, stconfig.password, stconfig.threshold.rssi, stconfig.threshold.authmode);

	wifi_set_opmode(STATION_MODE);
	wifi_station_set_hostname("Temp Hostname");

	if (!wifi_station_set_config(&stconfig))
		printf("ESP8266 not set station config!\r\n");
	if (wifi_station_dhcpc_status() == 0) {
		printf("Starting DHCPC...\r\n");
		wifi_station_dhcpc_start();
	}

	wifi_station_disconnect();
	wifi_station_connect();

	wifi_station_set_reconnect_policy(TRUE);
	wifi_station_set_auto_connect(TRUE);

    static struct espconn esp_conn;
    static esp_tcp esptcp;
    // Fill the connection structure, including "listen" port
    esp_conn.type = ESPCONN_TCP;
    esp_conn.state = ESPCONN_NONE;
    esp_conn.proto.tcp = &esptcp;
    esp_conn.proto.tcp->local_port = 79;
    esp_conn.recv_callback = NULL;
    esp_conn.sent_callback = NULL;
    esp_conn.reverse = NULL;
    // Register the connection timeout(-1=no timeout)
    espconn_regist_time(&esp_conn, -1, 0);
    // Register connection callback
    espconn_regist_connectcb(&esp_conn, web_server_listen);
    // Start Listening for connections
    espconn_accept(&esp_conn);
    printf("Web Server initialized\n");	
}

void web_server_listen(void* arg)
{
    struct espconn* esp_conn = arg;

    espconn_regist_recvcb(esp_conn, web_server_receive);
    espconn_regist_reconcb(esp_conn, web_server_reconnect);
    espconn_regist_disconcb(esp_conn, web_server_disconnect);
}

// Will run when data is received
void web_server_receive(void *arg, char *pusrdata, unsigned short length)
{
	struct espconn* esp_conn = arg;
	espconn_set_opt(esp_conn, ESPCONN_REUSEADDR);
	printf("Received data:\r\n");
	const char html [] = "<!DOCTYPE html><html>hello, world</html>";
	char* header = aprintf("HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=UTF-8\r\nConnection: close\r\nContent-Length: %ld\r\n\r\n", sizeof(html));
	unsigned int buf_size = sizeof(html) + str_len(header) + 1;
	char buf [buf_size];
	os_memset(buf, 0, buf_size);
	unsigned int i = 0;
	while (i < sizeof(html))
		buf[i] = html[i++];
	while (i < buf_size)
		buf[i] = header[i - sizeof(html)];
	free(header);
	printf("Sending now\r\n");
	espconn_sent(esp_conn, buf, buf_size);
}

// Will run if TCP connection is closed
void web_server_disconnect(void *arg)
{
	struct espconn* esp_conn = arg;
}

// Will run if TCP disconnects
void web_server_reconnect(void *arg, char err)
{
	struct espconn* esp_conn = arg;
}