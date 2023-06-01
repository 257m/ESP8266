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

void web_server_init(const char* ssid, const char* passwd, uint8_t channel, bool static_ip)
{
	struct station_config stconfig;
	str_cpy(stconfig.ssid, ssid, 32);
	str_cpy(stconfig.password, passwd, 64);
	stconfig.threshold.authmode = AUTH_WPA_WPA2_PSK;
	stconfig.threshold.rssi = -127;
	//stconfig.channel = channel;
   //stconfig.all_channel_scan = true;
	//stconfg.bssid = 0;

	printf("connecting to %s with %s\r\nthreshold rssi: %d auth: %d\r\n", stconfig.ssid, stconfig.password, stconfig.threshold.rssi, stconfig.threshold.authmode);

	wifi_set_opmode(STATION_MODE);
	wifi_station_set_hostname("ESP8266 Web Server");
	// I don't want to interrupted by UART while setting the wifi
	ETS_UART_INTR_DISABLE();
	if (!wifi_station_set_config(&stconfig))
		printf("ESP8266 not set station config!\r\n");
  wifi_station_disconnect();
	wifi_station_connect();
	// Reenable for obvious reasons 
	ETS_UART_INTR_ENABLE();
	wifi_set_channel(channel);
  if (!static_ip && wifi_station_dhcpc_status() == 0)
	  wifi_station_dhcpc_start();
  else if (wifi_station_dhcpc_status() == 1)
    wifi_station_dhcpc_stop();
	wifi_station_set_reconnect_policy(TRUE);
	wifi_station_set_auto_connect(TRUE);
    static struct espconn esp_conn;
    static esp_tcp esp_tcp;
    // Fill the connection structure, including "listen" port
    esp_conn.type = ESPCONN_TCP;
    esp_conn.state = ESPCONN_NONE;
    esp_conn.proto.tcp = &esp_tcp;
    esp_conn.proto.tcp->local_port = 9701;
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
    struct ip_info info;
    wifi_get_ip_info(STATION_IF, &info);
    printf("ip: %d:%d:%d:%d\tnetmask: %d:%d:%d:%d\tgw: %d:%d:%d:%d\r\n",
    ((unsigned char*)&info.ip)[0], ((unsigned char*)&info.ip)[1], ((unsigned char*)&info.ip)[2], ((unsigned char*)&info.ip)[3],
    ((unsigned char*)&info.netmask)[0], ((unsigned char*)&info.netmask)[1], ((unsigned char*)&info.netmask)[2], ((unsigned char*)&info.netmask)[3],
    ((unsigned char*)&info.gw)[0], ((unsigned char*)&info.gw)[1], ((unsigned char*)&info.gw)[2], ((unsigned char*)&info.gw)[3]);	
}

void web_server_listen(void* arg)
{
    struct espconn* esp_conn = arg;
	printf("Listening\n");
    espconn_regist_recvcb(esp_conn, web_server_receive);
    espconn_regist_reconcb(esp_conn, web_server_reconnect);
    espconn_regist_disconcb(esp_conn, web_server_disconnect);
}

// Will run when data is received
void web_server_receive(void *arg, char *pusrdata, unsigned short length)
{
	struct espconn* esp_conn = arg;
	espconn_set_opt(esp_conn, ESPCONN_REUSEADDR);
	printf("Received data:\r\n"); // Would print pusrdata but not sure if null terminated
	const char html [] = "<!DOCTYPE html><html>hello, world</html>";
	char* header_html = aprintf("HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=UTF-8\r\nConnection: close\r\nContent-Length: %ld\r\n\r\n%s", sizeof(html), html);
	printf("Sending now\r\n");
	espconn_sent(esp_conn, header_html, str_len(header_html));
	free(header_html);
}

// Will run if TCP connection is closed
void web_server_disconnect(void *arg)
{
	struct espconn* esp_conn = arg;

	printf("%d.%d.%d.%d:%d disconnect\n", esp_conn->proto.tcp->remote_ip[0],
            esp_conn->proto.tcp->remote_ip[1],esp_conn->proto.tcp->remote_ip[2],
            esp_conn->proto.tcp->remote_ip[3],esp_conn->proto.tcp->remote_port);
}

// Will run if TCP disconnects
void web_server_reconnect(void *arg, char err)
{
	struct espconn* esp_conn = arg;

	printf("%d.%d.%d.%d:%d err %d reconnect\n", esp_conn->proto.tcp->remote_ip[0],
        esp_conn->proto.tcp->remote_ip[1],esp_conn->proto.tcp->remote_ip[2],
        esp_conn->proto.tcp->remote_ip[3],esp_conn->proto.tcp->remote_port, err);
}
