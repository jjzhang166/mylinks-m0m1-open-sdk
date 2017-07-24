#ifndef __MYLINKS_WIFI_H
#define __MYLINKS_WIFI_H

struct station_config {
	unsigned char ssid[32];
	unsigned char password[64];
	unsigned char bssid_set;
	unsigned char bssid[6];
};



bool wifi_station_get_config(struct station_config *config);
bool wifi_station_set_config(struct station_config *config);

bool wifi_station_connect (void);
bool wifi_set_opmode(uint8_t opmode);
#endif

