#ifndef __MYLINKS_WIFI_H
#define __MYLINKS_WIFI_H

#include <lwip/ip_addr.h>
#include <wla_api.h>

struct station_config {
	unsigned char ssid[32];
	unsigned char password[64];
	unsigned char bssid_set;
	unsigned char bssid[6];
};

struct softap_config {
    unsigned char ssid[32];
    unsigned char password[64];
    unsigned char ssid_len;	// Note: Recommend to set it according to your ssid
    unsigned char channel;	// Note: support 1 ~ 13
    security_types authmode;
    unsigned char ssid_hidden;	// Note: default 0
    unsigned char max_connection;	// Note: default 4, max 4
    unsigned short beacon_interval;	// Note: support 100 ~ 60000 ms, default 100
};




extern bool wifi_station_get_config(struct station_config *config);
extern bool wifi_station_set_config(struct station_config *config);

extern bool wifi_station_connect (void);
extern bool wifi_set_opmode(uint8_t opmode);

extern bool wifi_get_ip_info(uint8_t if_index,struct ip_info *info);
extern bool wifi_set_ip_info(uint8_t if_index,struct ip_info *info);

//串口0输出
extern void uart0_sendStr(char *str);
extern void uart0_tx_buffer(const uint8_t *buf, uint16_t len);

/*
    if_index:STATION or SOFT_AP
    macaddr: 6 bytes
*/
extern bool wifi_get_macaddr(uint8_t if_index,uint8_t *macaddr);
extern bool wifi_set_macaddr(uint8_t if_index,uint8_t *macaddr);
#endif

