/*=============================================================================+
|                                                                              |
| Copyright 2014                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file Define all data structure of device.
*   \brief
*   \author Montage
*/

#ifndef LYNX_DEV_H
#define LYNX_DEV_H

#include <arch/cpu.h>
#include <arch/chip.h>
#include <arch/trap.h>
#include <mt_types.h>
#include <cmd.h>
#include <sdio.h>
#include <flash_api.h>
#include <mm.h>


#define BUF_LEN 512
#define USB_MAX_EP_NUM      4 //should be powers of 2
 
typedef union {
    u8     bdata[4];
    u16    sdata;
    u32    ldata;
} sdio_data_type;

struct lynx_dev {
	u32	current_sp;		/* Don't move it, stack pointer at here is easy to fetch */
#if (CONFIG_ROM_VER == 1)
	void (*os_schedule)(void);	 	/* DONT MOVE */
#endif
	irq_handler irq_tables[IRQ_MAX]; /* DONT MOVE */

	u32 sys_int_mask;
	u32 usec;
	u32 usec_overflow;
	u32 event_table;
	void (*raise_event)(int);
	u32	all_event;
	u32 current_event;
	void (*dsr_task_idle)(void);
	u32 sleep_msec;

	/* list for timeout handler */
	struct timeout_entry *timeout_list;
	struct timeout_entry *timeout_head;
	
	struct mm_t{
		u32 heapsize;
		struct mm_allocnode_s *heapstart[CONFIG_MM_REGIONS];
		struct mm_allocnode_s *heapend[CONFIG_MM_REGIONS];
		struct mm_freenode_s nodelist[MM_NNODES];
	}mm;
	/* Wiress MAC */
	struct wmac_dev{
		u16 wrx_bhdr_size;
		u16 free_bhdr_size;
		u16 extra_bhdr_size;
		u16 free_bhdr_head;
		u16 free_bhdr_tail;
	    u16 extra_bhdr_head;
		u16 extra_bhdr_tail;
		u16 free_bhdr_count;
		u16 extra_bhdr_count;
#if (CONFIG_ROM_VER == 1)
		u8 sta_tbl_count;
		u8 ds_tbl_count;

		u8 wrxq_max;
		u8 wrxq_idx;
		u8 wtxq_max;
		u8 wmac_is_ready;

		u32 int_mask;

		/*	Do not move order of below member.
			We need to configure register by this order */
		void *ext_sta_tbls;
		void *ext_ds_tbls;
		void *group_keys;
		void *private_keys;
		void *buf_headers;

		void *wl_rxq;
		void *wl_txq;
		u32 beacon_tx_bitmap;
		void *bufs;
		u32 ts_status;
		void *sw_buf_trace;
		void *sw_buf_trace_ra;
		void *extra_bufs;
		u8 beacon_rate_code;
		u8 beacon_change;
		u8 ps_uart;
#else
		u8 ps_uart;
		u8 res;
		void *bufs;
		void *buf_headers;
#endif
	}wmac;

	/* RF device */
	struct{
		u8 chip_ver;
		u8 power_level; /* RF power value */
		u8 if_type;
	}rf;
	
	u8	timeout_max_num;
	u8 	debug_level;

	u32 bb_reg_tbl;

	/* USB */
	struct usb_dev{
    	u8 ep0_state;
    	u8 usb_state;
    	u8 resume_state;
    	u8 speed;
		u8 non_ep0_num;
		u8 atdtw_loop;
		u8 reprime[2];

		u16 cfg_len;
		u16 device_pwr;
		void *dev_desc;
		void *cfg_desc;
		u32 endpoint_desc_ptr[6];
		void *str_desc;
		void *dev_qualifier;
		void *ubuf_base;
		void *s_qh;
		void *ub_list;
		void *head, *tail;

		u16 ubufq_count;
		u16 ubufq_max;
		u32 lock_flag;
		u32 udc_enable_intr;
		int (*rx_done)(void *ub, int len);
		int (*tx_done)(void *ub, int len);
		int (*ep0setup)(void *usbcmd);
		int (*udc_isr)(u32 irq_src);
		s16 cur_head[USB_MAX_EP_NUM];
		s16 last_idx[USB_MAX_EP_NUM];
		u8 setup_buf[12] __attribute__((aligned(0x8))) ; 
	}udev;

    /* SDIO */
    struct sdio_dev{
        u8 boot;
        u8 uhs;
        u16 cwder_cnt;
        u16 dwder_cnt;
        u16 poweron_delay;
#if (CONFIG_ROM_VER > 1)
        u16 vid;
        u16 pid;

        /* ring buffer meta data include buffer address and length */
        ring_buf_desc *ring;
#endif
        u32 irq_en;
        u32 cis_size;
        u32 cis_dmabase;
        u32 fun_dmabase;
#if (CONFIG_ROM_VER == 1)
        u32 des_dmabase;
#endif
        u32 db_start;
        u32 db_size;
#if (CONFIG_ROM_VER == 1)
        u32 rxd_num;
        u32 txd_num;
        struct sdio_desc *rxd_base;
        struct sdio_desc *txd_base;
        struct sdio_desc *rxd_cur;
        struct sdio_desc *txd_cur;
        struct sdio_desc *rxd_recycle;
        u8  *rxd_tbl;
        u8  *txd_tbl;
        u8  *bhdr_tbl;
        u16 bhdr_size;
        u8  enable_check_bhdr;
        u8  res;
        struct sdio_fw_info_tuple tpl_fw_info;
#endif
		int  (*from_host_done)(int, int, int);
#if (CONFIG_ROM_VER > 1)
            void (*reset)(int poweron, void (*check)(int, int *));
            int  (*to_host_done)(int head, int tail);
#endif
#if (CONFIG_ROM_VER == 1)
		int  (*to_host_done)(void);
		int  (*sdio_desc_init)(int);
		int  (*sdio_desc_deinit)(void);
#endif
		sdio_data_type data;
#if (CONFIG_ROM_VER > 1)
            /* hack into sdio setting via cmd until we run 'gos' */
            u8  bus_clock;
            u8  hs_support;
            u32 ocr;
#endif
    }sdev;

	u32 hrx_pkt_count;
	u32 htx_pkt_count;
	u32 htx_pkt_done_count;
	u32 hrx_nobuf_count;
	u32 hrx_refill_count;

	int (*host_event_handler)(void);
	int (*host_transmit_handler)(u32 head, u32 tail, u8 ep);
	void (*host_rxbuf_refill)(void);
	int (*generic_exception)(struct saved_frame *);
	void (*bhdr_get_handler)(int);
	void (*bhdr_insert_handler)(int, int);
	int (*serial_poll)(int chan, int *ch);
	void (*serial_io_char)(unsigned int);


	struct wrx_frame_info {
		struct wbuf 	*wb;
		u8				*fmhdr;
		u16				head;
		u16				tail;
		struct wm_bss	*bss;
		struct sta_ctx	*sta;
	}cur_wrx;

	struct hrx_frame_info {
		u16				head;
		u16				tail;
		struct lynx_cmd {
			u8	cmd_id;
			u8	seq_no;
			u16	len;
		} h2d_cmd;
	}cur_hrx;

	struct cm_t{
		int cmd_len;
		char *cmd_buf;
		u32 cmd_table[2];
		u8 	cmdt_sz[2];
		short state;
		unsigned long buf_address;
		unsigned long byte_count;
	}cmd;
	
#if (CONFIG_ROM_VER > 1)
	/* Sflash */
	struct sf_dev {
		u32 cr_init;
		struct flash_dev_tab fdb;
	}sf;

#else
	struct sf_dev {
		u32 id;
		u32 cr_init;
		u32 fdb;
	}sf;
#endif
	/* UART */
	struct ur_dev {
		u32 control_reg;
		u8	chan;
	}ur;

	u8 fw_download_state;
	u16 fw_download_cnt;
	u32 fw_download_dst;

	u8 static_buf[20]; /* for static buffer of inet_ntoa(), ether_ntoa(), and ether_aton() */
#if ((CONFIG_ROM_VER > 1)||defined(CONFIG_FLASH_BOOT))
	//OTP
	u8 *otp_bufp;
	u8 *overw_bufp;
#endif
#if ((CONFIG_ROM_VER > 1)||defined(CONFIG_FLASH_BOOT))
	void (*deep_sleep_func)(unsigned int msec);
	u8 syscall_arg;
	u8 os_tick;
#endif
};

register struct lynx_dev *ldev asm("r2");

/* timer handle definition */
struct timeout_entry {
	void (*timer_func)(void *);
	void *arg;
	unsigned int expire_time;
	struct timeout_entry *next;
};

enum {
	FW_WAIT = 0,
	FW_LOADING,
	FW_DOWNLOAD_DONE,
	FW_START,
	FW_RUNNING,
};

/* Note: host to device frame should be
		 struct lynx_cmd_header + 
		 struct lynx_cmd 		+ 
		 struct wci_cmd_xxx (if necessary & may be variate) */

enum WCI_CMD_ID {
	/* "1" reserved */

    /* managment cmd */
    WCI_H2D_MGT_TX = 2,
    WCI_H2D_MGT_TX_ACK,
    WCI_H2D_DEV_START,
    WCI_H2D_DEV_STOP = 5,
    WCI_H2D_DEV_SCAN = 6,
    WCI_H2D_ACCESS_MEMORY,
    WCI_H2D_GET_FW_VERSION,
    WCI_H2D_VIF_ATTACH,
    WCI_H2D_VIF_DETACH = 10,
    WCI_H2D_VIF_UPDATE,
	WCI_H2D_START_AP,
    WCI_H2D_STA_ADD,
    WCI_H2D_STA_REMOVE,
    WCI_H2D_STA_CONNECT = 15,
    WCI_H2D_STA_DISCONNECT,
    WCI_H2D_ALIVE,
	WCI_H2D_WDS_PEER_ADD,
    /* config cmd */
    WCI_H2D_SET_MAC,
    WCI_H2D_SET_CHANNEL = 20,
    WCI_H2D_SET_TX_POWER,
    WCI_H2D_SET_TXQ,
    WCI_H2D_SET_ERP,
    WCI_H2D_SET_HT,
    WCI_H2D_SET_KEY = 25,
    WCI_H2D_SET_DEF_KEY,
    WCI_H2D_SET_BSS_WEP,
    WCI_H2D_SET_BSS_PSK,
    WCI_H2D_SET_TSF,
    WCI_H2D_SET_BITRATE_MASK = 30,
    WCI_H2D_SET_BEACON,
    WCI_H2D_SET_SLOTTIME,
    WCI_H2D_SET_MON,
    WCI_H2D_SET_IE_POOL,
    WCI_H2D_SET_BSS_UPDATE = 35,
    WCI_H2D_SET_PMKSA, 
    WCI_H2D_SET_RTS_THRESHOLD,
    WCI_H2D_SET_2040_COEX,
    /* get info cmd */
    WCI_H2D_GET_VIF,
    WCI_H2D_GET_STA = 40,
    WCI_H2D_GET_TSF,
    WCI_H2D_GET_IES,
    WCI_H2D_GET_TX_POWER,
    WCI_H2D_GET_VIF_ADDR,
    WCI_H2D_GET_VIF_LINK_ST = 45,
	WCI_H2D_GET_MON,
	/* special control cmd */
	WCI_H2D_SEND_ADDBA,
	WCI_H2D_SET_MIN_RATEIDX,
	WCI_H2D_SET_PS,
	WCI_H2D_DBG_INFO_CMD,
	WCI_H2D_SET_MGMT_TXPKT = 125,
	WCI_H2D_SET_MGMT_IE = 126,
	WCI_H2D_MP_TEST_CMD = 127,
	/* data packet */
	WCI_H2D_PACKET = 128,
	/* reserve 128 ~ 135 for data packet per BSS */	
	WCI_H2D_PACKET_END = 135,
	WCI_H2D_FW_INFO = 250,
	WCI_H2D_FW_DOWNLOAD = 254,
	WCI_H2D_FW_COMP = 255,
};

enum WCI_RETURN_EVENT {
	WCI_D2H_TGT_RDY = 1,
	WCI_D2H_CONFIG_DONE,
	WCI_D2H_MGT_RX,
	WCI_D2H_FATAL,
	WCI_D2H_ADD_STA = 5,		/* return sta index */
	WCI_D2H_DEL_STA,			/* return sta index */
	WCI_D2H_ADDBA,
	WCI_D2H_DELBA,
	WCI_D2H_BAR,
	WCI_D2H_RX_MIC_ERR = 10,
	/* return 'get' data */
	WCI_D2H_RET_VIF,
	WCI_D2H_RET_STA,
	WCI_D2H_RET_TSF,
	WCI_D2H_RET_IES,
	WCI_D2H_RET_TXPOWER = 15,
	/* return cmd result */
	WCI_D2H_RET_VIF_ATTACH,
	WCI_D2H_RET_SCAN_STATE,
	WCI_D2H_RET_STA_CONNECTED,
	WCI_D2H_RET_STA_DISCONNECTED,
	WCI_D2H_RET_VIF_ADDR = 20,
	WCI_D2H_RET_VIF_LINK_ST,
	WCI_D2H_RET_MON,
	WCI_D2H_RET_DBG_INFO,

	WCI_D2H_RET_MP_TEST_CMD = 127,

	WCI_D2H_FW_INFO = 250,
};

#endif
