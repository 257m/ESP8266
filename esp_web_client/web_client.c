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
#include "web_client.h"

void wifi_handle_event(System_Event_t* evt)
{
	switch (evt->event) {
		case EVENT_STAMODE_CONNECTED:
			printf("connect to ssid %s, channel %d\n",
					evt->event_info.connected.ssid,
					evt->event_info.connected.channel);
			break;
		case EVENT_STAMODE_DISCONNECTED:
			printf("disconnect from ssid %s, reason %d\n",
					evt->event_info.disconnected.ssid,
					evt->event_info.disconnected.reason);
			break;
		case EVENT_STAMODE_AUTHMODE_CHANGE:
			printf("mode: %d -> %d\n",
					evt->event_info.auth_change.old_mode,
					evt->event_info.auth_change.new_mode);
			break;
		case EVENT_STAMODE_GOT_IP:
			printf("ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR,
					IP2STR(&evt->event_info.got_ip.ip),
					IP2STR(&evt->event_info.got_ip.mask),
					IP2STR(&evt->event_info.got_ip.gw));
			printf("\n");
			break;
		case EVENT_SOFTAPMODE_STACONNECTED:
			printf("station: " MACSTR "join, AID = %d\n",
					MAC2STR(evt->event_info.sta_connected.mac),
					evt->event_info.sta_connected.aid);
			break;
		case EVENT_SOFTAPMODE_STADISCONNECTED:
			printf("station: " MACSTR "leave, AID = %d\n",
					MAC2STR(evt->event_info.sta_disconnected.mac),
					evt->event_info.sta_disconnected.aid);
			break;
		default:
			break;
	}
}

static void ICACHE_FLASH_ATTR
web_client_sent(void* arg) {
	printf("---- SENDING ----\r\n");
	struct espconn* conn = arg;
	Request* req = conn->reverse;
	if (!req->post_data)
		return;
	espconn_sent(conn, (uint8_t *)req->post_data, str_len(req->post_data));
	free(req->post_data);
	req->post_data = NULL;
}

static void ICACHE_FLASH_ATTR
web_client_receive(void* arg, char* buf, unsigned short len)
{
	printf("---- RECEIVING ----\r\n");
	struct espconn* conn = arg;
	Request* req = conn->reverse;
	if (!req->buffer)
		req->buffer = malloc(len);
	else
		req->buffer = realloc(req->buffer, req->buffer_size + len);
	char* new_buf = req->buffer + req->buffer_size;
	req->buffer_size += len;
	while (new_buf < req->buffer + req->buffer_size - 1)
		*new_buf++ = *buf++;
	*new_buf = '\0';
}

static void ICACHE_FLASH_ATTR
web_client_connect(void* arg)
{
	printf("---- CONNECTED ----\r\n");
	struct espconn* conn = arg;
	Request* req = conn->reverse;
	espconn_regist_recvcb(conn, web_client_receive);
	espconn_regist_sentcb(conn, web_client_sent);

	char* buf = aprintf("%s %s HTTP/1.1\r\n"
						"Host: %s:%d\r\n"
						"Connection: close\r\n"
						"User-Agent: ESP8266\r\n"
						"%s"
						"Content-Length: %d\r\n"
						"\r\n",
						req->post_data ? "GET" : "POST", req->path, req->hostname, req->port, req->headers, req->post_data ? str_len(req->post_data) : 0);
	espconn_sent(conn, buf, str_len(buf));
	free(buf);
	free(req->headers);
	req->headers = NULL;
	printf("SENT REQUEST\r\n");
}

static void ICACHE_FLASH_ATTR
web_client_disconnect(void* arg)
{
	printf("---- DISCONNECTED ----\r\n");
	struct espconn* conn = arg;
	if (!conn)
		return;
	if (conn->reverse) {
		Request* req = conn->reverse;
		if (req->user_callback)
			req->user_callback(req);

		free(req->buffer);
		free(req->hostname);
		free(req->path);
		free(req);
	}
	espconn_delete(conn);
	if(conn->proto.tcp)
		free(conn->proto.tcp);
	free(conn);
}

static void ICACHE_FLASH_ATTR
web_client_reconnect(void* arg, char error)
{
	printf("---- CONNECTION ERROR ----\r\n");
	web_client_disconnect(arg);
}


void ICACHE_FLASH_ATTR
dns_callback(const char* hostname, ip_addr_t* addr, void* arg)
{
	struct espconn* conn = (struct espconn*)malloc(sizeof(struct espconn));
	Request* req = arg;
	conn->type = ESPCONN_TCP;
	conn->state = ESPCONN_NONE;
	conn->proto.tcp = (esp_tcp*)malloc(sizeof(esp_tcp));
	conn->proto.tcp->local_port = espconn_port();
	conn->proto.tcp->remote_port = req->port;
	conn->reverse = req;

  if (!addr) {
	  ip_addr_t dns_addr;
	  espconn_gethostbyname((struct espconn*)req, req->hostname, &dns_addr, dns_callback);
	  os_memcpy(conn->proto.tcp->remote_ip, &dns_addr, 4);
  }
  else
      os_memcpy(conn->proto.tcp->remote_ip, addr, 4);

	espconn_regist_connectcb(conn, web_client_connect);
	espconn_regist_disconcb(conn, web_client_disconnect);
	espconn_regist_reconcb(conn, web_client_reconnect);

	espconn_connect(conn);
}

void ICACHE_FLASH_ATTR
web_client_post(const char* url, const char* post_data, const char* headers, http_callback user_callback)
{
	Request* req = (Request*)malloc(sizeof(Request));
	char* port_str = str_find_char(url, ':');
	req->hostname = malloc(url - port_str);
	str_cpy(req->hostname, url, url - port_str);
	req->port = 80;
	if (port_str)
		req->port = atoi(port_str+1);
	req->path = str_find_char(url, '/');
	if (!req->path)
		req->path = str_find_char(url, '\0');
	req->secure = 0;
	req->headers = str_dup(headers);
	req->post_data = str_dup(post_data);
	req->buffer_size = 0;
	req->buffer = NULL;
	req->user_callback = user_callback;
	ip_addr_t addr;
	espconn_gethostbyname((struct espconn *)req, req->hostname, &addr, dns_callback);
	dns_callback(req->hostname, &addr, req);
}

void ICACHE_FLASH_ATTR
web_client_get(const char* url, const char* headers, http_callback onreceive)
{
	// Just a wrapper
	web_client_post(url, NULL, headers, onreceive);
}
void ICACHE_FLASH_ATTR
web_client_init(const char* ssid, const char* passwd, uint8_t channel, bool static_ip)
{
	wifi_set_opmode(STATION_MODE);
	//wifi_station_dhcps_stop();

	struct station_config stconfig;

	if (wifi_station_get_config(&stconfig)) {
		str_cpy(stconfig.ssid, ssid, 32);
		str_cpy(stconfig.password, passwd, 64);
		os_memcpy(stconfig.password, passwd, str_len(passwd));
		stconfig.threshold.authmode = AUTH_OPEN;
		// stconfig.threshold.ssid_hidden = 0;
		// stconfig.threshold.max_connection = 4;
		// stconfig.channel = channel;
		if (!wifi_station_set_config(&stconfig))
			printf("ESP8266 not set st config!\r\n");
	}

	wifi_set_event_handler_cb(wifi_handle_event);
	wifi_station_disconnect();
	wifi_station_connect();

	printf("Web Client initialized\n");
}
