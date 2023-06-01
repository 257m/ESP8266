#ifndef WEB_SERVER_H
#define WEB_SERVER_H

void web_server_init(const char* ssid, const char* passwd, uint8_t channel, bool static_ip);
extern void web_server_listen(void* arg);
extern void web_server_receive(void *arg, char *pusrdata, unsigned short length);
extern void web_server_disconnect(void *arg);
extern void web_server_reconnect(void *arg, char err);

#endif /* WEB_SERVER_H */
