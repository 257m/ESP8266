#ifndef WEB_SERVER_H
#define WEB_SERVER_H

typedef void (*http_callback)(char* response_body, int http_status, char* full_response);

typedef struct {
	char* path;
	int port;
	char* post_data;
	char* headers;
	char* hostname;
	char* buffer;
	int buffer_size;
	char secure;
	http_callback user_callback;
} Request;

extern void ICACHE_FLASH_ATTR web_client_init(const char* ssid, const char* passwd, uint8_t channel, bool static_ip);
extern void ICACHE_FLASH_ATTR web_client_get(char* url, char* headers, http_callback onreceive);

#endif /* WEB_SERVER_H */
