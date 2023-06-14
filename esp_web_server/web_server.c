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
#include "string.h"
#include "web_server.h"

static double temperature, pressure, pressure_altitude, density_altitude = 0;

void wifi_handle_event(System_Event_t* evt)
{
	switch (evt->event) {
		case EVENT_STAMODE_CONNECTED:
			PRINTF("connect to ssid %s, channel %d\n",
					evt->event_info.connected.ssid,
					evt->event_info.connected.channel);
			break;
		case EVENT_STAMODE_DISCONNECTED:
			PRINTF("disconnect from ssid %s, reason %d\n",
					evt->event_info.disconnected.ssid,
					evt->event_info.disconnected.reason);
			break;
		case EVENT_STAMODE_AUTHMODE_CHANGE:
			PRINTF("mode: %d -> %d\n",
					evt->event_info.auth_change.old_mode,
					evt->event_info.auth_change.new_mode);
			break;
		case EVENT_STAMODE_GOT_IP:
			PRINTF("ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR,
					IP2STR(&evt->event_info.got_ip.ip),
					IP2STR(&evt->event_info.got_ip.mask),
					IP2STR(&evt->event_info.got_ip.gw));
			PRINTF("\n");
			break;
		case EVENT_SOFTAPMODE_STACONNECTED:
			PRINTF("station: " MACSTR "join, AID = %d\n",
					MAC2STR(evt->event_info.sta_connected.mac),
					evt->event_info.sta_connected.aid);
			break;
		case EVENT_SOFTAPMODE_STADISCONNECTED:
			PRINTF("station: " MACSTR "leave, AID = %d\n",
					MAC2STR(evt->event_info.sta_disconnected.mac),
					evt->event_info.sta_disconnected.aid);
			break;
		default:
			break;
	}
}

// Will run when data is received
static void ICACHE_FLASH_ATTR
web_server_receive(void *arg, char *pusrdata, unsigned short length)
{
	struct espconn* esp_conn = arg;
	espconn_set_opt(esp_conn, ESPCONN_REUSEADDR);
	PRINTF("Received data:\r\n%s\r\n", pusrdata); // not sure if null terminated
	char* html = aprintf("<!DOCTYPE html><html><h1>WEATHER SENSING ROVER</h1><p>"
	                     "TEMPERATURE READING: %lf degrees<br>"
	                     "PRESSURE: %lf PA | %lf inHg<br>"
	                     "PRESSURE ALTITUDE: %lf ft<br>"
	                     "DENSITY ALTITUDE: %lf ft<br>"
	                     "</p></html>",
						 temperature, pressure, pressure_altitude, density_altitude;
	char* header_html = aprintf("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nConnection: keep-alive\r\nContent-Length: %d\r\n\r\n%s\r\n\r\n", str_len(html), html);
	PRINTF("Sending now\r\n");
	espconn_sent(esp_conn, header_html, str_len(header_html));
	free(header_html);
	free(html);
}

// Will run if TCP connection is closed
static void ICACHE_FLASH_ATTR
web_server_disconnect(void *arg)
{
	struct espconn* esp_conn = arg;

	PRINTF("%d.%d.%d.%d:%d disconnect\n", esp_conn->proto.tcp->remote_ip[0],
			esp_conn->proto.tcp->remote_ip[1],esp_conn->proto.tcp->remote_ip[2],
			esp_conn->proto.tcp->remote_ip[3],esp_conn->proto.tcp->remote_port);
}

// Will run if TCP disconnects
static void ICACHE_FLASH_ATTR
web_server_reconnect(void *arg, char err)
{
	struct espconn* esp_conn = arg;

	PRINTF("%d.%d.%d.%d:%d err %d reconnect\n", esp_conn->proto.tcp->remote_ip[0],
		esp_conn->proto.tcp->remote_ip[1],esp_conn->proto.tcp->remote_ip[2],
		esp_conn->proto.tcp->remote_ip[3],esp_conn->proto.tcp->remote_port, err);
}

static void ICACHE_FLASH_ATTR
web_server_listen(void* arg)
{
 	struct espconn* esp_conn = arg;
	PRINTF("Listening\n");
	espconn_regist_recvcb(esp_conn, web_server_receive);
	espconn_regist_reconcb(esp_conn, web_server_reconnect);
	espconn_regist_disconcb(esp_conn, web_server_disconnect);
}

void ICACHE_FLASH_ATTR
web_server_init(const char* ssid, const char* passwd, uint8_t channel, bool static_ip)
{
	wifi_set_opmode(SOFTAP_MODE);
	wifi_softap_dhcps_stop();

	struct softap_config apconfig;

	if (wifi_softap_get_config(&apconfig)) {
		str_cpy(apconfig.ssid, ssid, 32);
		str_cpy(apconfig.password, passwd, 64);
		os_memcpy(apconfig.password, passwd, os_strlen(passwd));
		apconfig.authmode = AUTH_OPEN;
		apconfig.ssid_hidden = 0;
		apconfig.max_connection = 4;
		// apconfig.channel=7;
		if (!wifi_softap_set_config(&apconfig))
			PRINTF("ESP8266 not set ap config!\r\n");
	}

	wifi_set_event_handler_cb(wifi_handle_event);
	static struct ip_info info;
	IP4_ADDR(&info.ip, 192, 168, 22, 1);
	IP4_ADDR(&info.gw, 192, 168, 22, 1);
	IP4_ADDR(&info.netmask, 255, 255, 255, 0);
	wifi_set_ip_info(SOFTAP_IF, &info);

	struct dhcps_lease dhcp_lease;
	IP4_ADDR(&dhcp_lease.start_ip, 192, 168, 22, 2);
	IP4_ADDR(&dhcp_lease.end_ip, 192, 168, 22, 5);
	wifi_softap_set_dhcps_lease(&dhcp_lease);

	wifi_softap_dhcps_start();

	PRINTF("SOFTAP Status:%d\r\n", wifi_softap_dhcps_status());
	PRINTF("Size of ESP8266: %d\r\n", sizeof(apconfig.ssid));
	PRINTF("Length of ESP8266: %d\r\n", os_strlen(apconfig.ssid));

	static struct espconn esp_conn;
	static esp_tcp esptcp;
	// Fill the connection structure, including "listen" port
	esp_conn.type = ESPCONN_TCP;
	esp_conn.state = ESPCONN_NONE;
	esp_conn.proto.tcp = &esptcp;
	esp_conn.proto.tcp->local_port = 80;
	esp_conn.recv_callback = NULL;
	esp_conn.sent_callback = NULL;
	esp_conn.reverse = NULL;
	// Register the connection timeout(-1=no timeout)
	espconn_regist_time(&esp_conn, 0, 0);
	// Register connection callback
	espconn_regist_connectcb(&esp_conn, web_server_listen);
	// Start Listening for connections
	espconn_accept(&esp_conn);
	PRINTF("Web Server initialized\n");
}
