/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*-----------------------------------------------------------------------------+
| Common Macros                                                                |
+-----------------------------------------------------------------------------*/
#if 0
#define CFG_MEM_1K   0x0400//1k = 1024
#define CFG_MEM_2K   0x0800//2k = 2048
#define CFG_MEM_4K   0x1000//4k = 4096
#define CFG_MEM_8K   0x2000//8k = 8192
#define CFG_MEM_16K  0x4000//16k = 16384
#endif

#define CFG_HDR_LEN           3

//for Boot from flash
//move the config flash memory start address to the last block of flash
#ifdef CONFIG_FLASH_BOOT
#define CFG_FLASH_MEM_START   0x70000
#else
#define CFG_FLASH_MEM_START   0x10000
#endif

#define mem_len_check(c)	(c == 0x4 || c == 0x8  || c == 0x10 \
									  || c == 0x20 || c == 0x40)//1k 2k 4k 8k 16k
#define isnum(c)       (c>=48 && c<=57) //ascii 0~9
#define hexupcase(c)   (c>=65 && c<=70) //ascii A~F
#define hexlowcase(c)  (c>=97 && c<=102)//ascii a~f

enum {
	CFG_LOAD	= (1 << 0),
	CFG_READ	= (1 << 1),
	CFG_WRITE	= (1 << 2),
	CFG_DUMP	= (1 << 3),
	CFG_SUBMIT	= (1 << 4),
	CFG_END		= (1 << 5),
};
extern unsigned short cfg_mem_size;

/*-----------------------------------------------------------------------------+
| CFG Data ID                                                                  |
+-----------------------------------------------------------------------------*/
enum {
	CFG_RGBLED = 1,				//STR
	CFG_DC_MOTOR,				//STR
	CFG_LED,					//STR
	CFG_SERVO_MOTOR,			//STR
	CFG_WIFI_BSS0,				//STR	//5
	CFG_WIFI_BSS1,				//STR
	CFG_WIFI_BSS2,				//STR
	CFG_WIFI_OPMODE,			//INT
	CFG_WIFI_PHY,				//INT
	CFG_WIFI_CHANNEL,			//INT	//10
	CFG_WIFI_BANDWIDTH,			//INT
	CFG_WIFI_TX_POWER,			//INT
	CFG_WIFI_AUTO_CONNECT,		//INT
	CFG_WIFI_RECONNECT,			//INT
	CFG_WIFI_MAC,				//STR	//15
	CFG_WIFI_DTIM,				//INT
	CFG_WIFI_HIDDEN_SSID,		//INT
	CFG_WIFI_SNIFFER,			//INT
	CFG_WIFI_SMART_CONFIG,		//STR
	CFG_WIFI_WPS,				//STR	//20
	CFG_WIFI_DHCPC,				//INT
	CFG_WIFI_DHCPS,				//INT
	CFG_WIFI_IP0,				//STR
	CFG_WIFI_IP1,				//STR
	CFG_WIFI_IP2,				//STR	//25
	CFG_WIFI_SSID0,				//STR
	CFG_WIFI_SSID1,				//STR
	CFG_WIFI_SSID2,				//STR
	CFG_SW_SDK,					//STR
	CFG_SW_REV,					//STR	//30
	CFG_SW_BUILD,				//STR
};

/*-----------------------------------------------------------------------------+
| Function Prototypes                                                          |
+-----------------------------------------------------------------------------*/
int config_load(void);
int config_get(unsigned char *des_bufp, int id);
int config_set(unsigned char *src_bufp, int id, int len);
int config_submit(void);
int config_end(void);


