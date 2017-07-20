/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*-----------------------------------------------------------------------------+
| OTP Data ID                                                                  |
+-----------------------------------------------------------------------------*/
enum {
	MAC_ADDR = 1,
	RFC1,
	RFC2,
	RFC3,
	TXLO,//5
	FOFS,
	TX_GAIN,
	SDIO_VEND_ID,
	SDIO_DEV_ID,
	USB_VEND_ID,//10
	USB_PROD_ID,
	CUSTOMER_ID,
	TIMEOUT_SEC,
	UART_CH,
	WATCHDOG_TIME,//15
	DEBUG_LEVEL,
	USB_MANU_STR,
	USB_PROD_STR,
	USB_SERI_STR,
	USB_CONF_STR,//20
	USB_INTF_STR,
	TXVGA_CH1,
	TXVGA_CH2,
	TXVGA_CH3,
	TXVGA_CH4,//25
	TXVGA_CH5,
	TXVGA_CH6,
	TXVGA_CH7,
	TXVGA_CH8,
	TXVGA_CH9,//30
	TXVGA_CH10,
	TXVGA_CH11,
	TXVGA_CH12,
	TXVGA_CH13,
	TXVGA_CH14,//35
	TEST = 62,
};

/*-----------------------------------------------------------------------------+
| Common Macros                                                                |
+-----------------------------------------------------------------------------*/
#define OTP_REG      0x000c0000
#define OTP_AVDD_REG 0x000c0004
#define AXI_CLK_MHZ  230
#define PGMEN_USEC   10

#define OTP_BYTE_SIZE         128
#define OTP_BIT_SIZE          (8 * OTP_BYTE_SIZE)
#define OTP_ADDR_SHIFT_BIT    8
#define OTP_CLK_PLUSE_WIDTH_SHIFT_BIT 21

#define OTP_READ_MODE_EN      0x00080000
#define OTP_PROGRAM_MODE_EN   0x00040000
#define OTP_ADDR_VALID_BIT    0x000003ff
#define OTP_DATA_VALID_BIT    0x000000ff
#define OTP_AVDD_EN           0x00000001
#define OTP_CLK_PLUSE_WIDTH_BIT11 0x00000002 // in OTP_AVDD_REG

#define OTP_DATA_TYPE_NUM   4
#define OTP_MEM_SIZE   128

#define isnum(c)       (c>=48 && c<=57) //ascii 0~9
#define hexupcase(c)   (c>=65 && c<=70) //ascii A~F
#define hexlowcase(c)  (c>=97 && c<=102)//ascii a~f

enum {
	OTP_LOAD	= (1 << 0),
	OTP_READ	= (1 << 1),
	OTP_WRITE	= (1 << 2),
	OTP_DUMP	= (1 << 3),
	OTP_SUBMIT	= (1 << 4),
	OTP_END		= (1 << 5),
};

extern const int otp_datalen[];
/*-----------------------------------------------------------------------------+
| Function Prototypes                                                          |
+-----------------------------------------------------------------------------*/
int otp_load(int mem_len);
int otp_read(unsigned char *des_bufp, int id, int len);
int otp_end(void);
int otp_check_space(void);


