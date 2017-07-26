/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file   wla_api.h
*   \brief  define wla API functions
*   \author Montage
*/

#ifndef _WLA_API_H_
#define _WLA_API_H_

#ifdef CONFIG_MICO
#include <MicoWlan.h>
#include <MICONotificationCenter.h>
#include <mico_wifi_monitor.h>
#endif

enum {
	NETIF_STA = 0,
	NETIF_AP,
};
/**
 *  @brief  wlan network interface enumeration definition.
 */
typedef enum {
	SOFT_AP,/**< Act as an access point, and other station can connect, 4 stations Max*/
	STATION	/**< Act as a station which can connect to an access point*/
} wlan_if_types;

/**
 *  @brief  Wi-Fi security type enumeration definition.
 */
typedef  enum {
	SECURITY_NONE,        /**< Open system. */
	SECURITY_WEP,         /**< Wired Equivalent Privacy. WEP security. */
	SECURITY_WPA_TKIP,    /**< WPA /w TKIP */
	SECURITY_WPA_AES,     /**< WPA /w AES */
	SECURITY_WPA2_TKIP,   /**< WPA2 /w TKIP */
	SECURITY_WPA2_AES,    /**< WPA2 /w AES */
	SECURITY_WPA2_MIXED,  /**< WPA2 /w AES or TKIP */
	SECURITY_AUTO,        /**< It is used when calling @ref wlan_start_adv, Wlan read security type from scan result. */
} security_types;

typedef enum {
	FILTER_RX_BEACON,     // RX wifi beacon
	FILTER_RX_PROBE_REQ,  // RX wifi probe request
	FILTER_RX_PROBE_RES,  // RX wifi probe response
	FILTER_RX_ACTION,     // RX wifi action packet
	FILTER_RX_MANAGEMENT, // RX all wifi management packet
	FILTER_RX_DATA,       // RX all wifi data packet
	FILTER_RX_MCAST_DATA, // RX all wifi the data packet which destination is broacast("FFFFFFFFFFFF") or IPv4 multicast MAC("01005Exxxxxx")
	FILTER_RX_CTRL,       // RX all wifi control packet
	FILTER_RX_MAX,
} filter_rx_types;

typedef enum {
	NOTIFY_WIFI_SCAN_COMPLETED,			//void (*function)(scan_result *pApList);
	NOTIFY_WIFI_STATUS_CHANGED,			//void (*function)(wifi_event status);
	NOTIFY_WIFI_PARA_CHANGED,			//void (*function)(apinfo_adv *ap_info, char *key, int key_len);
	NOTIFY_DHCP_COMPLETED,				//void (*function)(ip_sts *pnet);
	NOTIFY_DNS_RESOLVE_COMPLETED,		//void (*function)(char *str, int len);
	NOTIFY_WIFI_CONNECT_FAILED,			//void (*function)(int err);
	NOTIFY_WIFI_LED_TIMEOUT,			//int (*function)(void);
	/* User defined notifications */
} notify_types;

typedef enum {
	STATE_IDLE = 0,
	STATE_SCAN,
	STATE_SCAN_DONE,
	STATE_LINK_UP,
	STATE_LINK_DOWN,
} wlan_if_sm;

typedef enum {
	OPMODE_STA = 0,
	OPMODE_AP,
	OPMODE_APSTA,	// do not support.
} wlan_op_types;

typedef struct {
	char dhcp;		/**< DHCP mode: @ref DHCP_DISABLE, @ref DHCP_CLIENT, @ref DHCP_SERVER.*/
	char ip[16];	/**< Local IP address on the target wlan interface: @ref wlan_if_types.*/
	char gate[16];	/**< Router IP address on the target wlan interface: @ref wlan_if_types.*/
	char mask[16];	/**< Netmask on the target wlan interface: @ref wlan_if_types.*/
	char dns[16];	/**< DNS server IP address.*/
	char mac[16];	/**< MAC address, example: "C89346112233".*/
	char broadcastip[16];
} ip_sts;

struct _aplist_adv {
	char ssid[32];				/**< The SSID of an access point.*/
	char power;					/**< Signal strength, min:0, max:100*/
	unsigned char bssid[6];				/**< The BSSID of an access point.*/
	char channel;				/**< The RF frequency, 1-13*/
	security_types security;	/**< Security type, @ref security_types */
};

/**
 *  @brief  Scan result.
 */
typedef struct
{
	char ap_num;	/**< The number of access points found in scanning.*/
	void *aplist;
} scan_result;

/**
 *  @brief  Input network paras, used in wlan_start function.
 */
typedef struct
{
	char wifi_mode;				/**< Wi-Fi mode: @ref wlan_if_types.*/
	char wifi_ssid[32];			/**< SSID of the wlan needs to be connected.*/
	char wifi_key[64];			/**< Security key of the wlan needs to be connected, ignored in an open system.*/
	char local_ip_addr[16];		/**< Static IP configuration, Local IP address. */
	char net_mask[16];			/**< Static IP configuration, Netmask. */
	char gateway_ip_addr[16];	/**< Static IP configuration, Router IP address. */
	char dnssvr_ip_addr[16];	/**< Static IP configuration, DNS server IP address. */
	char dhcp_mode;				/**< DHCP mode, @ref DHCP_DISABLE, @ref DHCP_CLIENT and @ref DHCP_SERVER. */
	char reserved[32];
	int  wifi_retry_interval;	/**< Retry interval if an error is occured when connecting an access point, time unit is millisecond. */
} network_info;

/**
 *  @brief  Advanced precise wlan parameters, used in @ref network_info_adv.
 */
typedef struct
{
	char ssid[32];	/**< SSID of the wlan that needs to be connected. Example: "SSID String". */
	unsigned char bssid[6];	/**< BSSID of the wlan needs to be connected. Example: {0xC8 0x93 0x46 0x11 0x22 0x33}. */
	char channel;	/**< Wlan's RF frequency, channel 0-13. 1-13 means a fixed channel
							that can speed up a connection procedure, 0 is not a fixed input means all channels are possiable*/
	security_types security;
} apinfo_adv;

/**
 *  @brief  Input network precise paras in wlan_start_adv function.
 */
typedef struct
{
	apinfo_adv ap_info;			/**< @ref apinfo_adv. */
	char key[64];				/**< Security key or PMK of the wlan. */
	int  key_len;				/**< The length of the key. */
	char local_ip_addr[16];		/**< Static IP configuration, Local IP address. */
	char net_mask[16];			/**< Static IP configuration, Netmask. */
	char gateway_ip_addr[16];	/**< Static IP configuration, Router IP address. */
	char dnssvr_ip_addr[16];	/**< Static IP configuration, DNS server IP address. */
	char dhcp_mode;				/**< DHCP mode, @ref DHCP_DISABLE, @ref DHCP_CLIENT and @ref DHCP_SERVER. */
	char reserved[32];
	int  wifi_retry_interval;	/**< Retry interval if an error is occured when connecting an access point, time unit is millisecond. */
} network_info_adv;

/**
 *  @brief  Current link status in station mode.
 */
typedef struct {
	int is_connected;	/**< The link to wlan is established or not, 0: disconnected, 1: connected. */
	int wifi_strength;	/**< Signal strength of the current connected AP */
	char ssid[32];		/**< SSID of the current connected wlan */
	unsigned char bssid[6];		/**< BSSID of the current connected wlan */
	int  channel;		/**< Channel of the current connected wlan */
} link_sts;

typedef struct {
	unsigned char bssid[6];
	char ssid[33];
	char key[65];
	int  user_data_len;
	char user_data[65];
} omniconfig_result;

#ifdef CONFIG_MICO
#define NO_ERR							kNoErr

// ==== extention for  wlan ====
#define WLAN_NO_ERR						kWlanNoErr
#define WLAN_ERR_PENDING				kWlanPendingErr
#define WLAN_ERR_TIMEOUT				kWlanTimeoutErr
#define WLAN_ERR_RESULT					kWlanPartialResultsErr
#define WLAN_ERR_KEY					kWlanInvalidKeyErr
#define WLAN_ERR_NOTEXIST				kWlanNotExistErr
#define WLAN_ERR_AUTH					kWlanAuthenticatedErr
#define WLAN_ERR_NOTKEY					kWlanNotKeyedErr
#define WLAN_ERR_IOCTL					kWlanIOCtlErr
#define WLAN_NOT_BUFFER_READY_TEMP		kWlanBUFFER_UNAVAILABLE_TEMPORARY
#define WLAN_NOT_BUFFER_READY_PERM		kWlanUFFER_UNAVAILABLE_PERMANENT
#define WLAN_WPS_PBC_OVERLAP			kWlanWPS_PBC_OVERLAP
#define WLAN_CONN_LOST					kWlanConnectionLost


#define WLAN_ERR_GENERAL				kWlanGeneralErr
#define WLAN_ERR_ARG					kWlanArgErr
#define WLAN_ERR_OPTION					kWlanOptionErr
#define WLAN_NOT_UP						kWlanNotUp
#define WLAN_NOT_DOWN					kWlanNOotDown
#define WLAN_NOT_AP						kWlanNotAP
#define WLAN_NOT_STA					kWlanNotSTA
#define WLAN_ERR_KEYIDX					kWlanKeyIndexErr
#define WLAN_RADIO_OFF					kWlanRadioOff
#define WLAN_NOT_BANDLOCKED				kWlanNotBandLocked
#define WLAN_ERR_CLK					kWlanClkErr
#define WLAN_ERR_RATE					kWlanRateSetErr
#define WLAN_ERR_BAND					kWlanBandErr
#define WLAN_BUFFER_SHORT				kWlanBufTooShort
#define WLAN_BUFFER_LONG				kWlanBufTooLong
#define WLAN_BUSY						kWlanBusy
#define WLAN_NOT_ASSOCIATED				kWlanNotAssociated
#define WLAN_ERR_SSIDLEN				kWlanSSIDLenErr
#define WLAN_ERR_CHANNELRANGE			kWlanOutOfRangeChannelderr
#define WLAN_ERR_CHANNEL				kWlanChannelErr
#define WLAN_ERR_ADDR					kWlanAddressErr
#define WLAN_ERR_RESOURCES				kWlanResourcesErr
#define WLAN_UNSUPPORTED				kWlanUnUnsupported
#define WLAN_ERR_LENGTH					kWlanLengthErr
#define WLAN_ERR_NOTREADY				kWlanNotReadyErr
#define WLAN_ERR_NOTPERMIT				kWlanNotPermittedErr
#define WLAN_ERR_MEMORY					kWlanMemoryErr
#define WLAN_ASSOCIATED					kWlanAssociated
#define WLAN_ERR_RANGE					kWlanNotInRangeErr
#define WLAN_ERR_NOTFOUND				kWlanNotFoundErr
#define WLAN_NOT_WME					kWlanWMENotEnabled
#define WLAN_NOT_TSPEC					kWlanTSPECNotFound
#define WLAN_NOT_ACM					kWlanACMNotSupported
#define WLAN_NOT_WMEASSOCIATED			kWlanNOTWMEAssociation
#define WLAN_ERR_SDIOBUS				kWlanSDIOBusErr
#define WLAN_NOT_ACCESSIBLE				kWlanNotAccessible
#define WLAN_ERR_VERSION				kWlanVersionErr
#define WLAN_ERR_TX						kWlanTXErr
#define WLAN_ERR_RX						kWlanRXErr
#define WLAN_ERR_NODEVICE				kWlanNoDeviceErr
#define WLAN_ERR_UNFINISHED				kWlanUnFinishedErr
#define WLAN_DISABLED					kWlanDisabled
#define WLAN_ERR_LAST					kWlanErrLast

#define DHCP_DISABLE					DHCP_Disable
#define DHCP_CLIENT						DHCP_Client
#define DHCP_SERVER						DHCP_Server

typedef enum {
	NOTIFY_STA_LINK_UP = NOTIFY_STATION_UP,
	NOTIFY_STA_LINK_DOWN = NOTIFY_STATION_DOWN,

	NOTIFY_AP_LINK_UP = NOTIFY_AP_UP,
	NOTIFY_AP_LINK_DOWN = NOTIFY_AP_DOWN,
} wifi_event;

#else /* CONFIG_MICO */

#if 0
#include <stdbool.h>
#endif

#define NO_ERR							0		//! No error occurred.

// ==== extention for  wlan ====
#define WLAN_NO_ERR						0		/**< No error occurred in wlan operation. */
#define WLAN_ERR_PENDING				1		/**< Pending. */
#define WLAN_ERR_TIMEOUT				2		/**< Timeout occurred in wlan operation. */
#define WLAN_ERR_RESULT					3		/**< Partial results */
#define WLAN_ERR_KEY					4		/**< Invalid key */
#define WLAN_ERR_NOTEXIST				5		/**< Does not exist */
#define WLAN_ERR_AUTH					6		/**< Not authenticated */
#define WLAN_ERR_NOTKEY					7		/**< Not keyed */
#define WLAN_ERR_IOCTL					8		/**< IOCTL fail */
#define WLAN_NOT_BUFFER_READY_TEMP		9		/**< Buffer unavailable temporarily */
#define WLAN_NOT_BUFFER_READY_PERM		10		/**< Buffer unavailable permanently */
#define WLAN_WPS_PBC_OVERLAP			11		/**< WPS PBC overlap */
#define WLAN_CONN_LOST					12		/**< Connection lost */


#define WLAN_ERR_GENERAL				-1		/**< General error in wlan operation. */
#define WLAN_ERR_ARG					-2		/**< Wlan parameter is incorrect, missing, or not appropriate. */
#define WLAN_ERR_OPTION					-3		/**< Bad option */
#define WLAN_NOT_UP						-4		/**< Not up */
#define WLAN_NOT_DOWN					-5		/**< Not down */
#define WLAN_NOT_AP						-6		/**< Not AP */
#define WLAN_NOT_STA					-7		/**< Not STA  */
#define WLAN_ERR_KEYIDX					-8		/**< BAD Key Index */
#define WLAN_RADIO_OFF					-9		/**< Radio Off */
#define WLAN_NOT_BANDLOCKED				-10		/**< Not  band locked */
#define WLAN_ERR_CLK					-11		/**< No Clock */
#define WLAN_ERR_RATE					-12		/**< BAD Rate valueset */
#define WLAN_ERR_BAND					-13		/**< BAD Band */
#define WLAN_BUFFER_SHORT				-14		/**< Buffer too short */
#define WLAN_BUFFER_LONG				-15		/**< Buffer too long */
#define WLAN_BUSY						-16		/**< Busy */
#define WLAN_NOT_ASSOCIATED				-17		/**< Not Associated */
#define WLAN_ERR_SSIDLEN				-18		/**< Bad SSID len */
#define WLAN_ERR_CHANNELRANGE			-19		/**< Out of Range Channel */
#define WLAN_ERR_CHANNEL				-20		/**< Bad Channel */
#define WLAN_ERR_ADDR					-21		/**< Bad Address */
#define WLAN_ERR_RESOURCES				-22		/**< Not Enough Resources */
#define WLAN_UNSUPPORTED				-23		/**< Unsupported */
#define WLAN_ERR_LENGTH					-24		/**< Bad length */
#define WLAN_ERR_NOTREADY				-25		/**< Not Ready */
#define WLAN_ERR_NOTPERMIT				-26		/**< Not Permitted */
#define WLAN_ERR_MEMORY					-27		/**< No Memory */
#define WLAN_ASSOCIATED					-28		/**< Associated */
#define WLAN_ERR_RANGE					-29		/**< Not In Range */
#define WLAN_ERR_NOTFOUND				-30		/**< Not Found */
#define WLAN_NOT_WME					-31		/**< WME Not Enabled */
#define WLAN_NOT_TSPEC					-32		/**< TSPEC Not Found */
#define WLAN_NOT_ACM					-33		/**< ACM Not Supported */
#define WLAN_NOT_WMEASSOCIATED			-34		/**< Not WME Association */
#define WLAN_ERR_SDIOBUS				-35		/**< SDIO Bus Error */
#define WLAN_NOT_ACCESSIBLE				-36		/**< WLAN Not Accessible */
#define WLAN_ERR_VERSION				-37		/**< Incorrect version */
#define WLAN_ERR_TX						-38		/**< TX failure */
#define WLAN_ERR_RX						-39		/**< RX failure */
#define WLAN_ERR_NODEVICE				-40		/**< Device not present */
#define WLAN_ERR_UNFINISHED				-41		/**< To be finished */
#define WLAN_DISABLED					-43		/**< Disabled in this build */
#define WLAN_ERR_LAST					-44

#define DHCP_DISABLE	(0)	/**< Disable DHCP service. */
#define DHCP_CLIENT		(1)	/**< Enable DHCP client which get IP address from DHCP server
								automatically, reset Wi-Fi connection if failed. */
#define DHCP_SERVER		(2)	/**< Enable DHCP server, needs assign a static address as local address. */

typedef enum {
	NOTIFY_STA_LINK_UP = 1,
	NOTIFY_STA_LINK_DOWN,

	NOTIFY_AP_LINK_UP,
	NOTIFY_AP_LINK_DOWN,
} wifi_event;

#endif /* CONFIG_MICO */

typedef void (*monitor_cb_t)(unsigned char *data, int len);
/*
  * Initialize Wi-Fi basic settings.
  */
void wlan_init(void);

/** @defgroup Wlan Functions
  * @{
  */

/** @brief  Connect or establish a Wi-Fi network in normal mode (station or soft ap mode).
 *
 *  @detail This function can establish a Wi-Fi connection as a station or create
 *          a soft AP that other stations can connect (4 stations Max). In station mode,
 *          Wlan first scan all of the supported Wi-Fi channels to find a wlan that
 *          matches the input SSID, and read the security mode. Then try to connect
 *          to the target wlan. If any error occurs in the connection procedure or
 *          disconnected after a successful connection, Wlan start the reconnection
 *          procedure in background after a time interval defined in inNetworkInitPara.
 *          Call this function twice when setup coexistence mode (station + soft ap).
 *          This function returns immediately in station mode, and the connection will
 *          be executed in background.
 *
 *  @param  net: Specifies wlan parameters.
 *
 *  @return In station mode, always return WLAN_NO_ERR.
 *          In soft ap mode, return WLANXXXERR
 */
int wlan_start(network_info *net);

/** @brief  Connect to a Wi-Fi network with advantage settings (station mode only)
 *
 *  @detail This function can connect to an access point with precise settings,
 *          that greatly speed up the connection if the input settings are correct
 *          and fixed. If this fast connection is failed for some reason, Wlan
 *          change back to normal: scan + connect mode refer to @ref wlan_start.
 *          This function returns after the fast connection try.
 *
 *  @note   This function cannot establish a soft ap, use wlan_start() for this
 *          purpose.
 *          If input SSID length is 0, Wlan use BSSID to connect the target wlan.
 *          If both SSID and BSSID are all wrong, the connection will be failed.
 *
 *  @param  net: Specifies the precise wlan parameters.
 *
 *  @return Always return WLAN_NO_ERR although error occurs in first fast try.
 *          Return WLAN_ERR_TIMEOUT if DHCP client timeout.
 */
int wlan_start_adv(network_info_adv *net);

/** @brief  Read current wireless link status.
 *
 *  @param  sts: Point to the buffer to store the link status.
 *
 *  @retval NO_ERR.
 */
int wlan_get_link_sts(link_sts *sts, wlan_if_types type);

/** @brief  Start a wlan scanning in 2.4GHz in background.
 *
 *  @detail Once the scan is completed, Wlan sends a notify:
 *          NOTIFY_WIFI_SCAN_COMPLETED, with callback function:
 *          void (*function)(scan_result *pApList, Context_t * const inContext).
 *          Register callback function using @ref add_notification() before scan.
 */
void wlan_start_scan(void);

/** @brief  Close the RF chip's power supply, all network connection is lost.
 *
 *  @return NO_ERR: succeed\n WLAN_ERR_GENERAL: failed
 */
int wlan_power_off(void);

/** @brief  Open the RF's power supply and do some necessary initialization.
 *
 *  @note   The defaut RF state is powered on after @ref wlan_init, so this function is
 *          not needed after wlan_init.
 *
 *  @return NO_ERR: succeed\n WLAN_ERR_GENERAL: failed
 */
int wlan_power_on(void);

/**@brief  Close all the Wi-Fi connections, station mode and soft ap mode.
 *
 * @note   This function also stop the background retry mechanism started by
 *         wlan_start() and wlan_start_adv().
 *
 * @return NO_ERR.
 */
int wlan_suspend(void);

/** @brief  Close the connection in station mode.
 *<F7>
 *  @note   This function also stop the background retry mechanism started by
 *          wlan_start() and wlan_start_adv().
 *
 *  @retval NO_ERR.
 */
int wlan_suspend_station(void);

/** @brief  Stop soft ap and close all stations' connections.
 *
 *  @return NO_ERR.
 */
int wlan_suspend_softap(void);

#if 0
/** @brief  Start WPS configuration procedure
 *
 *  @param  inTimeout: If WPS is excuted longer than this parameter in backgound.
 *          Wlan stops WPS and sends a callback where nwkpara is NULL
 *
 *  @retval NO_ERR.
 */
int wlan_start_wps(int timeout);

/** @brief  Stop WPS configuration procedure
 *
 *  @retval NO_ERR.
 */
int wlan_stop_wps(void);


/** @brief  Start wechat airkiss configuration procedure
 *
 *  @detail This function can read SSID, password and extra user data sent from
 *          Easylink APP.
 *          Wlan sends a callback: NOTIFY_OMNICONFIG_WPS_COMPLETED
 *          with function:
 *          void (*function)(network_InitTypeDef_st *nwkpara, Context_t * const inContext);
 *          that provide SSID and password, nwkpara is NULL if timeout or get an error
 *
 *  @param  inTimeout: If airkiss is excuted longer than this parameter in backgound.
 *          Wlan stops airkiss and sends a callback where nwkpara is NULL
 *
 *  @retval NO_ERR.
 */
int wlan_start_airkiss(int timeout);

/** @brief  Stop wechat airkiss configuration procedure
 *
 *  @retval NO_ERR.
 */
int wlan_stop_airkiss(void);
#endif

/** @brief  Enable IEEE power save mode
 *
 *  @detail When this function is enabled, Wlan enter IEEE power save mode if
 *          Wlan is in station mode and has connected to an AP, and do not need
 *          any other control from application. To save more power, use
 *          @ref mcu_powersave_config
 */
int wlan_enable_powersave(void);

/**
 * @brief  Disable IEEE power save mode
 */
int wlan_disable_powersave(void);

/*
  * Set which wifi packet will be captured.
  * RX all wifi packet if didn't call this function.
  * This function can be called more than once to set RX different type packet.
  */
int wlan_monitor_rx_type(filter_rx_types type);

/*
  * Start wifi monitor.
  */
int wlan_start_monitor(void);

/*
  * Stop wifi monitor.
  */
int wlan_stop_monitor(void);

/*
  * Set TX power level.
  */
int wlan_set_txpwr(int level);

/*
  * Set Wi-Fi physical mode.
  */
int wlan_set_phy(int phy);

/*
  * Set the monitor channel. Valid channel is 1~13
  */
int wlan_set_channel(int channel);

/*
  * Set the monitor channel and bandwith. Valid channel is 1~13 
  *.Set ht40 as 0 for 20Mhz ,ht40 as 1 for 40Mhz.
  */
int wlan_set_ch_bandwith(int channel,int ht40);

/*
  * Set the callback function to RX the captured wifi packet.
  */
void wlan_register_monitor_cb(monitor_cb_t fn);


int wlan_init_notification(void);
int wlan_add_notification(notify_types type, void *functionAddress);
int wlan_del_notification(notify_types type);
int wlan_del_notification_all(notify_types type);

/*
  * Set Wi-Fi network MAC address.
  */
void wlan_set_myaddr(wlan_if_types type, char *myaddr);

/*
  * Get Wi-Fi network MAC address.
  */
char *wlan_get_myaddr(wlan_if_types type);

/*
  * Set Wi-Fi network hidden ssid status (ap mode only).
  */
void wlan_set_hidden_ssid(int en);

/*
  * Get Wi-Fi network hidden ssid status (ap mode only).
  */
int wlan_get_hidden_ssid(void);

/*
  * Set reconnect policy in STA mode.
  */
void wlan_set_reconnect(int en);

/*
  * Get reconnect policy in STA mode.
  */
int wlan_get_reconnect(void);
/*
  * Get Wi-Fi network connection number.
  */
int wlan_get_sta_num(wlan_if_types type);
/*
  * Gets Wi-Fi station information (ap mode only).
  */
int wlan_get_sta_info(int (*get_mac_callback)(const char *));
/*
  * Gets Wi-Fi connection status.
  */
int wlan_get_ifs_sm(wlan_if_types type);
/*
  * Dump all APs information into apps buffer.
  */
void wlan_scan_result_to_buffer(void);
/*
  * The function starts to service Wi-Fi LED callback function.
  */
void wlan_led_install(void);
/*
  * The function stop to service Wi-Fi LED callback function.
  */
void wlan_led_uninstall(void);

#define WIFI_RST_PIN	2
#define WIFI_LED_PIN	15

#endif
