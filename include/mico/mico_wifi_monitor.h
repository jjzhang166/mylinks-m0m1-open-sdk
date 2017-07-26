#ifndef _MICO_WIFI_MONITOR_H
#define _MICO_WIFI_MONITOR_H

#include "stdint.h"

typedef void (*monitor_cb_t)(uint8_t*data, int len);
typedef enum {
	WLAN_FILTER_RX_BEACON,     // RX wifi beacon
	WLAN_FILTER_RX_PROBE_REQ,  // RX wifi probe request
	WLAN_FILTER_RX_PROBE_RES,  // RX wifi probe response
	WLAN_FILTER_RX_ACTION,     // RX wifi action packet
	WLAN_FILTER_RX_MANAGEMENT, // RX all wifi management packet
	WLAN_FILTER_RX_DATA,       // RX all wifi data packet
	WLAN_FILTER_RX_MCAST_DATA, // RX all wifi the data packet which destination is broacast("FFFFFFFFFFFF") or IPv4 multicast MAC("01005Exxxxxx")

	WLAN_FILTER_MAX,
} FILTER_RX_TYPE_E;

/*
  * Set which wifi packet will be captured.
  * RX all wifi packet if didn't call this function.
  * This function can be called more than once to set RX different type packet.
  */
int mico_wlan_monitor_rx_type(FILTER_RX_TYPE_E type);

/*
  * Start wifi monitor. 
  */
int mico_wlan_start_monitor(int mode);

/*
  * Stop wifi monitor. 
  */
int mico_wlan_stop_monitor(void);

/*
  * Set the monitor channel. Valid channel is 1~13
  */
int mico_wlan_set_channel(int channel);

/*
  * Set the monitor channel. Valid channel is 1~13
  *.Set ht40 as 0 for 20Mhz ,ht40 as 1 for 40Mhz.
  */
int mico_wlan_set_ch_bandwith(int channel,int ht40);

/*
  * Set the callback function to RX the captured wifi packet.
  */
void mico_wlan_register_monitor_cb(monitor_cb_t fn);

#endif

