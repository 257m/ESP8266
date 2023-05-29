#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#define SSID "WRDSB Wireless"
#define PASSWORD ""

extern void web_server_init();
extern void web_server_listen(void* arg);
extern void web_server_receive(void *arg, char *pusrdata, unsigned short length);
extern void web_server_disconnect(void *arg);
extern void web_server_reconnect(void *arg, char err);

#endif /* WEB_SERVER_H */