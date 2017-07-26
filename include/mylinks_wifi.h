#ifndef __MYLINKS_WIFI_H
#define __MYLINKS_WIFI_H

struct station_config {
	unsigned char ssid[32];
	unsigned char password[64];
	unsigned char bssid_set;
	unsigned char bssid[6];
};



extern bool wifi_station_get_config(struct station_config *config);
extern bool wifi_station_set_config(struct station_config *config);

extern bool wifi_station_connect (void);
extern bool wifi_set_opmode(uint8_t opmode);




//串口0输出
extern void uart0_sendStr(char *str);
extern void uart0_tx_buffer(const uint8_t *buf, uint16_t len);
#endif

