/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*-----------------------------------------------------------------------------+
| Common Macros                                                                |
+-----------------------------------------------------------------------------*/

//for Boot from flash
//move the config flash memory start address to the last block of flash

#define CFG_FLASH_ENCRYPT_START 0x10000
#define CFG_FLASH_MEM_LENGTH 0x1000

#define CFG_FLASH_RF_START   (CFG_FLASH_ENCRYPT_START + CFG_FLASH_MEM_LENGTH)
//#define CFG_FLASH_MEM_START   (CFG_FLASH_RF_START + 0x1000)
#define CFG_FLASH_MEM_START  (CFG_FLASH_RF_START + CFG_FLASH_MEM_LENGTH)
#define CFG_FLASH_MEM2_START  (CFG_FLASH_MEM_START + CFG_FLASH_MEM_LENGTH)
#define CFG_FLASH_BOOTINFO_START (CFG_FLASH_MEM2_START + CFG_FLASH_MEM_LENGTH)
#define CFG_FLASH_WEBKEY_START (CFG_FLASH_BOOTINFO_START + CFG_FLASH_MEM_LENGTH)

#define CFG_MAGIC 0x6658
#define SEC_CFG_MAGIC 0x20150724
#define START_MAGIC_NUMBER 0x19870906
#undef s8  // conflict with arch/cpu.h
typedef __signed char s8;
#undef u8
typedef unsigned char u8;
#undef u16
typedef unsigned short u16;
#undef u32
typedef unsigned int u32;

typedef struct
{
	u8 enable;
	u8 duty;
}dc_moter_param;

typedef struct
{
	u8 enable;
	u8 angle;
}servo_moter_param;

typedef struct
{
	u8 enable;
	u8 r;
	u8 g;
	u8 b;
}rgbled_param;

typedef struct
{
	unsigned int baudrate;
	unsigned char  databits;
	unsigned char  stopbits;
	unsigned char  parity;
	unsigned char  flowctrl;
}uart_param;

typedef struct
{
	char ssid[33];
	char key[65];
	char channel;
	char enc;
	unsigned char bssid[6];
}sta_param;


typedef struct
{
	//u32 ip;/**< Static IP configuration, Local IP address. */
	char ip[16];		/**< Static IP configuration, Local IP address. */
	//u32 mask;			/**< Static IP configuration, Netmask. */
	char mask[16];		/**< Static IP configuration, Netmask. */
	//u32 gw;				/**< Static IP configuration, Router IP address. */
	char gw[16];		/**< Static IP configuration, Router IP address. */
}staip_param;

typedef struct
{
	char ssid[33];
	char key[65];
	char channel;
	char enc;
	char max_con;
	char hidden_ssid;
}softap_param;


typedef struct
{
	char ip[16];		/**< Static IP configuration, Local IP address. */
	char mask[16];		/**< Static IP configuration, Netmask. */
	char gw[16];		/**< Static IP configuration, Router IP address. */
}softapip_param;


typedef struct
{
	unsigned char sta_mac[6];
	unsigned char ap_mac[6];
}mac_param;

typedef struct
{
	char startip[16];
	char endip[16];
	int leave_time;
}dhcps_param;

#define DNS_BUF_LEN 64

typedef struct
{
	u8 enable;			// 0:cancel trans mode when start;1:start
	char ip[DNS_BUF_LEN];
	int port;
	//char type[4];
	u8 type;
	int time_alive;
}trans_param;

typedef struct
{
	char sw_sdk[64];
	char sw_rev[64];
	char sw_build[64];
}ver_info;

typedef struct
{
	u8 name[32];
	u8 key[32];
}webkey_param;


typedef struct
{
	u16 check_sum;//用于判断配置文件是否正确
	u8 led_value;
	u8 dhcp_mode;
	//u8 save_param;
	servo_moter_param sm_param;
	dc_moter_param dm_param;
	mac_param mac;
	uart_param ur_param;
	staip_param staip_param;
	softapip_param apip_param;
	dhcps_param dhs_param;
	trans_param socka;//socka
	trans_param sockb;
	u8 echoFlag;
	u8 tran_flag;
	webkey_param webkeyParam;
	//ver_info  ver_param;
	int cfg_magic;
}sdk_param;





typedef struct
{
	u16 check_sum;//用于判断配置文件是否正确
	u8 auto_conn;
	u8 reconn;
	u8 work_mode;
	u8 phy_mode;
	//u8 save_param;
	u8 txpower;
	sta_param sta_param;
	softap_param ap_param;
	//ver_info  ver_param;
	int cfg_magic;
}rf_param;


typedef struct
{
	unsigned short check_sum;//用于判断配置文件是否正确
	u8 txpower;
	sta_param sta_param;
	mac_param mac;
	unsigned int boot;
	int cfg_magic;
}alink_rf_param;

typedef struct
{
	u32 boot;
}boot_param;



struct platform_sec_flag_param {
    u32 magic_num;
    u8 key[6];
};


/*-----------------------------------------------------------------------------+
| Function Prototypes                                                          |
+-----------------------------------------------------------------------------*/
int config_load(void);
int config_get(sdk_param *param);
int config_set(sdk_param *param);
int config_submit(void);
int config_erase(void);
int rf_config_erase(void);

#define MACSTRP "%02X:%02X:%02X:%02X:%02X:%02X"
#define MACSTRT "%02X-%02X-%02X-%02X-%02X-%02X"
#define MACSTRS "%02X%02X%02X%02X%02X%02X"

#ifndef MAC2STR
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"


#define DNS_FLAG 0x4
#define NO_DNS	0x0
#define DNS_TYPE 0x4
#define SERVER 	0x0
#define CLIENT	0x2
#define SERVER_TYPE 0x2
#define TCP		0x0
#define UDP 	0x1
#define PORTO_TYPE 0x1

#define TCP_SERVER (TCP|SERVER)
#define TCP_CLIENT (TCP|CLIENT)
#define UDP_SERVER (UDP|SERVER)
#define UDP_CLIENT (UDP|CLIENT)

#define CHECK_PROTOCOL(type) (type&(PORTO_TYPE|SERVER_TYPE))
#define IS_DNS(type) ((type & DNS_TYPE) == DNS_FLAG)

enum tel_off{
	IPCLOSED = 0,
	RECLOSE = 1,
	IPCONNECTD = 2,
	RECONNECT = 3,
	IPCLOSEING = 4,
	IPCONNECTING = 5,
	IPCONNECT = 6,
	//DNS_CONNECT = 7,
	//DNS_RECONNECT = 8 ,
	TCPSERVERCLOSE = 9,
};

extern sdk_param g_atParam;
extern rf_param g_rfParam;

#endif




