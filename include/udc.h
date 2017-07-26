/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
#ifndef __UDC_H__
#define __UDC_H__
#include <mt_types.h> //for using u8, etc..
#include <arch/chip.h>
#include <mt_udc.h>
#include <cdc.h>
/*-----------------------------------------------------------------------------+
| Debug Information Macros                                                     |
+-----------------------------------------------------------------------------*/
#define usbinf(fmt,args...) \
        DBG_PRINTF(INFO, "%s() "fmt"\n",__FUNCTION__,##args)
#define usberr(fmt,args...) \
        DBG_PRINTF(ERROR, "%s() "fmt"\n",__FUNCTION__,##args)
#define usbdbg(fmt,args...) \
        //serial_printf("[Debug]+%d %s: "fmt"\n",__LINE__,__FUNCTION__,##args)
/*-----------------------------------------------------------------------------+
| Common Macros                                                                |
+-----------------------------------------------------------------------------*/
#define min( a, b ) ((a) < (b) ? (a) : (b))
#define udc_readl(x)     (*((volatile u32 *)(x)))
#define udc_writel(a, b) (*((volatile u32 *)(a)) = ((volatile u32)b))
/*-----------------------------------------------------------------------------+
| Bit Offset Macros                                                            |
+-----------------------------------------------------------------------------*/
#define BIT0            0x00000001
#define BIT1            0x00000002
#define BIT2            0x00000004
#define BIT3            0x00000008
#define BIT4            0x00000010
#define BIT5            0x00000020
#define BIT6            0x00000040
#define BIT7            0x00000080

#define BIT8            0x00000100
#define BIT9            0x00000200
#define BIT10           0x00000400
#define BIT11           0x00000800
#define BIT12           0x00001000
#define BIT13           0x00002000
#define BIT14           0x00004000
#define BIT15           0x00008000

#define BIT16           0x00010000
#define BIT17           0x00020000
#define BIT18           0x00040000
#define BIT19           0x00080000
#define BIT20           0x00100000
#define BIT21           0x00200000
#define BIT22           0x00400000
#define BIT23           0x00800000

#define BIT24           0x01000000
#define BIT25           0x02000000
#define BIT26           0x04000000
#define BIT27           0x08000000
#define BIT28           0x10000000
#define BIT29           0x20000000
#define BIT30           0x40000000
#define BIT31           0x80000000
/*-----------------------------------------------------------------------------+
| Endpoint Parameters                                                          |
+-----------------------------------------------------------------------------*/
#define HS_BULK_MAXPKTSIZE 512
#define FS_BULK_MAXPKTSIZE 64
#define CTRL_MAXPKTSIZE    64

/* endpoint modifiers
 * static struct usb_endpoint_description function_default_A_1[] = {
 *
 *     {this_endpoint: 0, attributes: CONTROL,   max_size: 8,  polling_interval: 0 },
 *     {this_endpoint: 1, attributes: BULK,  max_size: 64, polling_interval: 0, direction: IN},
 *     {this_endpoint: 2, attributes: BULK,  max_size: 64, polling_interval: 0, direction: OUT},
 *     {this_endpoint: 3, attributes: INTERRUPT, max_size: 8,  polling_interval: 0},
 *
 *
 */
#define OUT         0x00
#define IN          0x80

#define CONTROL     0x00
#define ISOCHRONOUS 0x01
#define BULK        0x02
#define INTERRUPT   0x03


/* configuration modifiers */
#define BMATTRIBUTE_RESERVED        0x80
#define BMATTRIBUTE_SELF_POWERED    0x40
#define BMATTRIBUTE_REMOTE_WAKEUP   0x20

/*-----------------------------------------------------------------------------+
| USB Definition Code                                                          |
+-----------------------------------------------------------------------------*/
/* Device and/or Interface Class codes */
#define USB_CLASS_PER_INTERFACE         0       /* for DeviceClass */
#define USB_CLASS_AUDIO                 1
#define USB_CLASS_COMM                  2
#define USB_CLASS_HID                   3
#define USB_CLASS_PHYSICAL              5
#define USB_CLASS_PRINTER               7
#define USB_CLASS_MASS_STORAGE          8
#define USB_CLASS_HUB                   9
#define USB_CLASS_DATA                  10
#define USB_CLASS_APP_SPEC              0xfe
#define USB_CLASS_VENDOR_SPEC           0xff

/* USB types */
#define USB_TYPE_STANDARD               (0x00 << 5)
#define USB_TYPE_CLASS                  (0x01 << 5)
#define USB_TYPE_VENDOR                 (0x02 << 5)
#define USB_TYPE_RESERVED               (0x03 << 5)

/* USB recipients */
#define USB_RECIP_DEVICE                0x00
#define USB_RECIP_INTERFACE             0x01
#define USB_RECIP_ENDPOINT              0x02
#define USB_RECIP_OTHER                 0x03

/* USB directions */
#define USB_DIR_OUT                     0x00
#define USB_DIR_IN                      0x80

/* Descriptor types */
#define USB_DT_DEVICE                    0x01
#define USB_DT_CONFIGURATION             0x02
#define USB_DT_STRING                    0x03
#define USB_DT_INTERFACE                 0x04
#define USB_DT_ENDPOINT                  0x05
#define USB_DT_DEVICE_QUALIFIER          0x06
#define USB_DT_OTHER_SPEED_CONFIGURATION 0x07
#define USB_DT_INTERFACE_POWER           0x08
#define USB_DT_HID                      (USB_TYPE_CLASS | 0x01)
#define USB_DT_REPORT                   (USB_TYPE_CLASS | 0x02)
#define USB_DT_PHYSICAL                 (USB_TYPE_CLASS | 0x03)
#define USB_DT_HUB                      (USB_TYPE_CLASS | 0x09)

/* Descriptor sizes per descriptor type */
#define USB_DT_DEVICE_SIZE              18
#define USB_DT_CONFIG_SIZE              9
#define USB_DT_INTERFACE_SIZE           9
#define USB_DT_ENDPOINT_SIZE            7
#define USB_DT_ENDPOINT_AUDIO_SIZE      9       /* Audio extension */
#define USB_DT_HUB_NONVAR_SIZE          7
#define USB_DT_HID_SIZE                 9

/* Endpoints */
#define USB_ENDPOINT_NUMBER_MASK        0x0f    /* in bEndpointAddress */
#define USB_ENDPOINT_DIR_MASK           0x80

#define USB_ENDPOINT_XFERTYPE_MASK      0x03    /* in bmAttributes */
#define USB_ENDPOINT_XFER_CONTROL       0
#define USB_ENDPOINT_XFER_ISOC          1
#define USB_ENDPOINT_XFER_BULK          2
#define USB_ENDPOINT_XFER_INT           3

/* Standard requests */
#define USB_REQ_GET_STATUS              0x00
#define USB_REQ_CLEAR_FEATURE           0x01
#define USB_REQ_SET_FEATURE             0x03
#define USB_REQ_SET_ADDRESS             0x05
#define USB_REQ_GET_DESCRIPTOR          0x06
#define USB_REQ_SET_DESCRIPTOR          0x07
#define USB_REQ_GET_CONFIGURATION       0x08
#define USB_REQ_SET_CONFIGURATION       0x09
#define USB_REQ_GET_INTERFACE           0x0A
#define USB_REQ_SET_INTERFACE           0x0B
#define USB_REQ_SYNCH_FRAME             0x0C
#define USB_REQ_DOWNLOAD_APP_DOING      0xFE
#define USB_REQ_DOWNLOAD_APP_DONE       0xFF

/* USB Spec Release number */
#define USB_BCD_VERSION                 0x0200

/* Device Requests (setup command) */
#define USB_REQ_DIRECTION_MASK          0x80
#define USB_REQ_TYPE_MASK               0x60
#define USB_REQ_RECIPIENT_MASK          0x1f

#define USB_REQ_DEVICE2HOST             0x80
#define USB_REQ_HOST2DEVICE             0x00

#define USB_REQ_TYPE_STANDARD           0x00
#define USB_REQ_TYPE_CLASS              0x20
#define USB_REQ_TYPE_VENDOR             0x40

#define USB_REQ_RECIPIENT_DEVICE        0x00
#define USB_REQ_RECIPIENT_INTERFACE     0x01
#define USB_REQ_RECIPIENT_ENDPOINT      0x02
#define USB_REQ_RECIPIENT_OTHER         0x03

#define USB_REQ_DEVICE2HOST_CLASS_INTERFACE  (USB_REQ_DEVICE2HOST + USB_REQ_TYPE_CLASS + USB_REQ_RECIPIENT_INTERFACE) //0xa1
#define USB_REQ_HOST2DEVICE_CLASS_INTERFACE  (USB_REQ_HOST2DEVICE + USB_REQ_TYPE_CLASS + USB_REQ_RECIPIENT_INTERFACE) //0x21

/* get status bits */
#define USB_STATUS_SELFPOWERED          0x01
#define USB_STATUS_REMOTEWAKEUP         0x02

#define USB_STATUS_HALT                 0x01

/* standard feature selectors */
#define USB_ENDPOINT_HALT               0x00
#define USB_DEVICE_REMOTE_WAKEUP        0x01
#define USB_TEST_MODE                   0x02

/* USB device speeds */
#define USB_SPEED_FULL      (PORTSCX_PORT_SPEED_FULL >> PORTSCX_SPEED_BIT_POS)  /* 12Mbps */
#define USB_SPEED_LOW       (PORTSCX_PORT_SPEED_LOW >> PORTSCX_SPEED_BIT_POS) /* 1.5Mbps */
#define USB_SPEED_HIGH      (PORTSCX_PORT_SPEED_HIGH >> PORTSCX_SPEED_BIT_POS) /* 480Mbps */
#define USB_SPEED_RESERVED  (PORTSCX_PORT_SPEED_UNDEF >> PORTSCX_SPEED_BIT_POS)
/*-----------------------------------------------------------------------------+
| Standard USB Descriptor Structures                                           |
+-----------------------------------------------------------------------------*/
#define	NON_EP0_NUM	3		/* EP1_IN, EP1_OUT, EP2_OUT */ 

struct usb_endpoint_descriptor {
    u8 bLength;
    u8 bDescriptorType; /* 0x5 */
    u8 bEndpointAddress;
    u8 bmAttributes;
    u16 wMaxPacketSize;
    u8 bInterval;
} __attribute__((packed));

struct usb_interface_descriptor {
    u8 bLength;
    u8 bDescriptorType; /* 0x04 */
    u8 bInterfaceNumber;
    u8 bAlternateSetting;
    u8 bNumEndpoints;
    u8 bInterfaceClass;
    u8 bInterfaceSubClass;
    u8 bInterfaceProtocol;
    u8 iInterface;
} __attribute__((packed));

struct usb_configuration_descriptor {
    u8 bLength;
    u8 bDescriptorType; /* 0x2 */
    u16 wTotalLength;
    u8 bNumInterfaces;
    u8 bConfigurationValue;
    u8 iConfiguration;
    u8 bmAttributes;
    u8 bMaxPower;
} __attribute__((packed));

struct usb_device_descriptor {
    u8 bLength;
    u8 bDescriptorType; /* 0x01 */
    u16 bcdUSB;
    u8 bDeviceClass;
    u8 bDeviceSubClass;
    u8 bDeviceProtocol;
    u8 bMaxPacketSize0;
    u16 idVendor;
    u16 idProduct;
    u16 bcdDevice;
    u8 iManufacturer;
    u8 iProduct;
    u8 iSerialNumber;
    u8 bNumConfigurations;
} __attribute__((packed));

struct usb_string_descriptor {
    u8 bLength;
    u8 bDescriptorType; /* 0x03 */
    u16 wData[0];
} __attribute__((packed));

struct usb_qualifier_descriptor {
    u8  bLength;
    u8  bDescriptorType;

    u16 bcdUSB;
    u8  bDeviceClass;
    u8  bDeviceSubClass;
    u8  bDeviceProtocol;
    u8  bMaxPacketSize0;
    u8  bNumConfigurations;
    u8  bRESERVED;
} __attribute__((packed));

struct usb_generic_descriptor {
    u8 bLength;
    u8 bDescriptorType;
    u8 bDescriptorSubtype;
} __attribute__((packed));

/* descriptor union structures */
struct usb_descriptor {
    union {
        struct usb_generic_descriptor generic;
        struct usb_endpoint_descriptor endpoint;
        struct usb_interface_descriptor interface;
        struct usb_configuration_descriptor configuration;
        struct usb_device_descriptor device;
        struct usb_string_descriptor string;
    } descriptor;

} __attribute__((packed));


/* USB Requests */
struct usb_device_request {
        u8 bmRequestType;
        u8 bRequest;
        u16 wValue;
        u16 wIndex;
        u16 wLength;
} __attribute__((packed, aligned(0x4)));

struct total_string_desc {
    struct usb_string_descriptor string_desc;
    char string[64];
} __attribute__((packed));

struct udc {
    u32 ep0_state;
    u32 usb_state;
    u32 resume_state;
    u32 speed;
};

enum usb_device_state {
    /* NOTATTACHED isn't in the USB spec, and this state acts
     * the same as ATTACHED ... but it's clearer this way.
     */
    USB_STATE_NOTATTACHED = 0,

    /* chapter 9 and authentication (wireless) device states */
    USB_STATE_ATTACHED,
    USB_STATE_POWERED,          /* wired */
    USB_STATE_RECONNECTING,     /* auth */
    USB_STATE_UNAUTHENTICATED,  /* auth */
    USB_STATE_DEFAULT,          /* limited function */
    USB_STATE_ADDRESS,
    USB_STATE_CONFIGURED,       /* most functions */

    USB_STATE_SUSPENDED
};
enum ep0_state {
    EP0_NONE,
    //EP0_SETUP,
    EP0_DATA,
    EP0_STATUS
};
enum ubuf_state	/*shrink UB_FREE, UB_ALLOC, UB_RELEASE from adad, acac, abab to ad, ac, ab */
{
    UB_FREE = 0xad,
    UB_ALLOC = 0xac,
    UB_RELEASE = 0xab,
    UB_OK = 0,
    UB_ERR_ID = 3,
    UB_HEAD_SZ = 0x40,
};

enum ep_setup_cmd
{
	EP_SETUP_CMD_ENABLE_QH,
	EP_SETUP_CMD_LINK_TD,
	EP_SETUP_CMD_FREE_TD_BUF,
	EP_SETUP_CMD_PKT_MAXSIZE,	
};

struct boot_total_cfg_desc {
    struct usb_configuration_descriptor configuration_desc;
    struct usb_interface_descriptor     interface_desc;//control interface
    struct usb_endpoint_descriptor      endpoint_desc[NON_EP0_NUM];
} __attribute__((packed));

#define STR_LANGID       0x0
#define STR_MANUFACTURER 0x1
#define STR_PRODUCT      0x2
#define STR_SERIAL       0x3
#define STR_CONFIG       0x4
#define STR_INTERFACE    0x5
#define STR_COUNT        0x6

extern const struct usb_device_descriptor boot_dev_desc;
extern const struct boot_total_cfg_desc boot_cfg_descs;
extern const struct total_string_desc boot_str_descs[STR_COUNT];
extern const struct usb_qualifier_descriptor boot_dev_qualifier;
/*-----------------------------------------------------------------------------+
| Function Prototypes                                                          |
+-----------------------------------------------------------------------------*/
void udc_rxbuf_refill(void);
int  udc_tx_done(void *, int);
int  udc_rx_done(void *, int);
int  udc_rx_dwfw_done(void *, int);
int  default_udc_tx_done(void *, int);
void boot_enpt_descs_setting(void);
int udc_transmit(unsigned int head, unsigned int tail, u8 ep_num);
struct ubuf *udc_ep_request(u32 ep_offset, u32 src, int len, int (*cb)(void *, int), u32 arg);
void clear_pending_frame(void);
void usb_otp_id(int option, int vend_id, int prod_id, char *otp_str, int len);

typedef int (*UDC_RX_FUNCPTR)(void *, int);
#endif /* __UDC_H__ */
