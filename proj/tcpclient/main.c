/*=============================================================================+
|                                                                              |
| Copyright 2015                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file at_command_demo.c
*   \brief main entry
*   \author Montage
*/

/*=============================================================================+
| Included Files                                                               |
+=============================================================================*/
#include <stdint.h>
#include <common.h>
#include <flash_api.h>
#include "lwip/sockets.h"
#include <lynx_debug.h>
#include <gpio.h>
#include <serial.h>
#include <event.h>
#include <otp.h>
#include <os_api.h>
#include <net_api.h>
#include <wla_api.h>
#include <gpio.h>
#include <version.h>
#include <built_info.h>
#include <cfg_api_new.h>
#include <user_config.h>

#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#if defined(CONFIG_LWIP)
#include <net_api.h>
#endif
#if defined(CONFIG_MINIFS)
#include <minifs/mfs.h>
#endif
#include <cJSON/cJSON.h>

#define TEST_UART   0//Lynx UART1
#define uart_log(...)                       serial_printf(__VA_ARGS__)
#define UART_RECV_TIMEOUT                   0
#define UART_ONE_PACKAGE_LENGTH             1024
#define UART_BAUD_RATE                      115200
#define MAGIC_NUM							0x1
struct message_header
{
	uint8_t id[12];
	uint32_t type;
	uint32_t check;
};

enum M0_state {
	M0_REGISTER_OK = 1,
	M0_DEV_GET_DATA,
	M0_DEV_SET_NOW_DATA,
	M0_DEV_SET_SAVE_DATA,
	M0_DEV_SET_ONOFF,
	M0_DEV_HEART,
	M0_REGISTER_APP,
	M0_TELL_DEV_DATA_CHANGE,
	M0_JSON_ERR,
	M0_USER_ERR,
	M0_TIMES_ERR,
	M0_MAC_READY,
	M0_DATA_MAX,
};

extern char conStatus[MEMP_NUM_NETCONN];
const char *sw_build_time = (char *)SW_BUILD_TIME;
const char *sw_build_sdk = (char *)MT_SDK_VER_STR;
const int sw_build_count = SW_BUILD_COUNT;

sdk_param g_atParam;
extern struct serial_buffer *ur1_rxbuf;
uint16_t magic_user = 0;
char *update_url = NULL;
uint8_t update_flag = 0;

static rf_param g_def_Myrfparam =
{
	0,				//check_sum;
	1, 				//auto connect
	1, 				//reconn
	OPMODE_STA,		//work mode
	7, 				//phy mode
	12,				//txpower
	//劢领智能办公室路由器
	{
		"Mylinks",				//ssid
		"welcometomylinks", 	//key
		//0,
		//SECURITY_AUTO,
		//{0x0,0x0,0x0,0x0,0x0,0x0},
		7,
		SECURITY_WPA2_MIXED,
		{0xf4,0x83,0xcd,0x59,0xf9,0x2d},
	},
	{
		"M0M100D0_",	//ap ssid
		"",				//ap key
		7,				//channel
		SECURITY_NONE,	//enc
		4,
		0,
	},

	CFG_MAGIC,		//HEAD
};



/*----------------------------------------------------------------*/
/**
 * The function is called once application start-up. Users can initial
 * structures or global parameters here.
 *
 * @param None.
 * @return int Protothread state.
 */
/*----------------------------------------------------------------*/
int app_main(void)
{
#ifdef CONFIG_WLA_LED
	pin_mode(WIFI_LED_PIN, 1);
	digital_write(WIFI_LED_PIN, 0);
#endif
	/* Do not add any process here, user should add process in user_thread */
	hw_sys_init();
	//serial_conf(8, 0, 1, 1,0 );
	return PT_EXITED;
}




void uart_init(void)
{
	int threshold = 0;	
	serial_conf(7, 0, 1, TEST_UART,0);
	serial_init((int)TEST_UART);

#ifdef CONFIG_UR_FLOW_CONTROL
	pin_mode(CONFIG_UR_CTS, 0);
	digital_write(CONFIG_UR_RTS, 0);//RTS active
	pin_en_intr(CONFIG_UR_CTS, 0, (gpio_func_cb)uart_flow_cts_stat, NULL);//0: rising
	pin_en_intr(CONFIG_UR_CTS, 1, (gpio_func_cb)uart_flow_cts_stat, NULL);//1: falling
	pin_mode(CONFIG_UR_RTS, 1);
#endif	
}




uint8_t net_init_start(trans_param *s){
	return s->enable?CONNECT:IPCLOSED;
}


static void mylinks_notify_wifi_para_handler(apinfo_adv *ap_info, char *key, int key_len)
{
#if VER_DEBUG
	serial_printf("SSID=%s BSSID=%s CH=%d SEC=%d KEY=%s KEY_Len=%d\n",
				ap_info->ssid, ether_ntoa(ap_info->bssid),
				ap_info->channel, ap_info->security,
				key, key_len);
#else
	printf("SSID=%s BSSID=%s CH=%d SEC=%d KEY=%s KEY_Len=%d\n",
				ap_info->ssid, ether_ntoa(ap_info->bssid),
				ap_info->channel, ap_info->security,
				key, key_len);
#endif
	//连接的路由器BSSID是WIFI模块AP的，去除
	if(!memcmp(ap_info->bssid,wlan_get_myaddr(SOFT_AP),6)){
		return;
	}
	//连接的路由器BSSID是WIFI模块STATION的，去除
	if(!memcmp(ap_info->bssid,wlan_get_myaddr(STATION),6)){
		return;
	}
	if(!ap_info->channel){
		g_rfParam.sta_param.channel = 0;
		start_ap_at_apsta_mode();
		return;
	}
	//如果没有加密，但是有密码长度
	if(!ap_info->security && key_len){
		return;
	}

	//如果设置的连接的路由器密码和账号与回调中的不同，则要重新再来一次。
	if(strcmp(ap_info->ssid,g_rfParam.sta_param.ssid)||strcmp(key,g_rfParam.sta_param.key)){
		apsta_restart();
		return;
	}

	memcpy(g_rfParam.sta_param.bssid,ap_info->bssid,6);
	g_rfParam.sta_param.enc = ap_info->security;
	g_rfParam.sta_param.channel = ap_info->channel;
	return;
	
}

static void Mylinks_cmd_thread(void *arg)
{

//	struct serial_buffer *pbuf = ur1_rxbuf;
	for(;;){
		if(!uart_recv_sem_wait()){
			uart_rev_proc();
			//while(serial_buffer_empty(pbuf)){
			//	buf[temp] = serial_buffer_getchar(pbuf);
			//}
		}
		if(ldev->wmac.ps_uart != 2){
			ldev->wmac.ps_uart = 0;
		}
	}
	vTaskDelete(NULL);
}


int8_t check_mac(uint8_t *mac){
	uint8_t i;
	for(i = 0;i<12;i++){
		if(mac[i]>='0' && mac[i]<='9'){
			continue;
		}
		if(mac[i]>='A' && mac[i]<='F'){
			continue;
		}
		if(mac[i]>='a' && mac[i]<='f'){
			continue;
		}
		return -1;
	}
	return 0;
}


static void save_device(uint32_t magic_num,uint8_t *sta_mac){
	struct platform_sec_flag_param flag;
	flag.magic_num = magic_num;
	flag.key[0] = 'y'^sta_mac[5];
	flag.key[1] = 'u'^sta_mac[2];
	flag.key[2] = 'n'^sta_mac[0];
	flag.key[3] = 'h'^sta_mac[3];
	flag.key[4] = 'a'^sta_mac[1];
	flag.key[5] = 'o'^sta_mac[4];
	flash_erase(CFG_FLASH_ENCRYPT_START, CFG_FLASH_MEM_LENGTH);
	flash_write(CFG_FLASH_ENCRYPT_START,(unsigned int)&flag, sizeof(struct platform_sec_flag_param));
	return;
}

static uint8_t AsciiToNum(uint8_t a){
	if(a >='0' && a<='9') return (a - '0');
	if(a >='A' && a<='F') return (a - 'A' + 10);
	if(a >='a' && a<='f') return (a - 'a' + 10);
	return a;
}

static int8_t upgade_otp_mac(uint8_t *mac){
	int otp_ret;
	otp_ret = otp_load(OTP_MEM_SIZE);
	if(OTP_MEM_SIZE!=otp_ret){
		return -1;
	}
	otp_ret = otp_write(mac, MAC_ADDR, 6);
	if(otp_ret == OTP_MEM_SIZE || otp_ret == 0)//OTP write not enough memory
    {
		return -1;
    }
	if(!otp_submit()){
		return -1;
	}
	otp_end();
	return 0;
}

int8_t encryption_save(struct message_header *s)
{
	uint8_t sta_mac[6];
	uint32_t magic_num = SEC_CFG_MAGIC;
	sta_mac[0] = (AsciiToNum(s->id[0]) << 4) + AsciiToNum(s->id[1]);
	sta_mac[1] = (AsciiToNum(s->id[2]) << 4) + AsciiToNum(s->id[3]);
	sta_mac[2] = (AsciiToNum(s->id[4]) << 4) + AsciiToNum(s->id[5]);
	sta_mac[3] = (AsciiToNum(s->id[6]) << 4) + AsciiToNum(s->id[7]);
	sta_mac[4] = (AsciiToNum(s->id[8]) << 4) + AsciiToNum(s->id[9]);
	sta_mac[5] = (AsciiToNum(s->id[10]) << 4) + AsciiToNum(s->id[11]);
	magic_num = (Formula_CRC16(sta_mac,6) << 16) | Formula_CRC16("YunHao",6) | (Formula_CRC16((uint8_t *)&magic_num,4) << 7);
	if(sw32(s->type) == M0_REGISTER_OK){
		//更新otp中的MAC地址
		uart0_sendStr("更新MAC地址......");
		if(upgade_otp_mac(sta_mac) < 0){
			uart0_sendStr("失败!\r\n");
			return -1;
		}
		uart0_sendStr("成功!\r\n");
	}
	save_device(magic_num,sta_mac);
	return 0;
}

int user_thread(void *arg )
{
	//int recvlen;
	//int i = 0;
	uint8_t tmp[64];
	struct message_header Msg;
	struct sockaddr *from;
	link_sts linkStatus;
	int32_t sock_fd;
	int32_t ret;
	uint16_t magic_id = 0;
	struct sockaddr_in *server_addr;
	struct sockaddr_in *tcp_from;
	int nNetTimeout = 300;
	uint8_t bssid[6];
	int16_t numchars = 0;
	socklen_t fromlen = sizeof(struct sockaddr_in);
	PMUREG(PKG_MODE_CTRL) |= STATION_MODE;
	//64K
	mylinks_flash_init(1);
	load_def_config(&g_atParam);
	memcpy(&g_rfParam,&g_def_Myrfparam,sizeof(rf_param));

#ifdef CONFIG_LWIP
	tcpip_init(0, 0);
	net_init_notification();
#endif
	wlan_init_notification();
	mylinks_gpio_init();
	wlan_init();
	uart_init();
	uart0_rev_register(Mylinks_cmd_thread);
	
#ifdef CONFIG_PING
	ping_init();
#endif
	mylinks_init_notification();
	wlan_add_notification(NOTIFY_WIFI_PARA_CHANGED, mylinks_notify_wifi_para_handler);
	wlan_led_install();
	uart0_sendStr("\r\n\r\n***劢领智能生产测试软件:");
	uart0_sendStr(sw_build_sdk);
	uart0_sendStr(LVERSTRING);
    uart0_sendStr("***\r\n");
	apsta_init();
	uart0_sendStr("当前Flash:8Mbit\r\n");
	while(get_slinkup() != STA_LINK_GET_IP) sys_msleep(10);
	wlan_get_link_sts(&linkStatus, STATION);
	sprintf(tmp,"信号强度:%d\r\n",linkStatus.wifi_strength);
	uart0_sendStr(tmp);
	memset(tmp,0,sizeof(tmp));
	from = (struct sockaddr *)zalloc(sizeof(struct sockaddr));
	server_addr = (struct sockaddr_in *)zalloc(sizeof(struct sockaddr_in));
	memset(server_addr, 0, sizeof(struct sockaddr_in));
	server_addr->sin_family = AF_INET;
	server_addr->sin_addr.s_addr = inet_addr("118.178.87.170");
	server_addr->sin_port = htons(17330);
	server_addr->sin_len = sizeof(struct sockaddr_in);
	tcp_from = server_addr;
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	for(;;){
		setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));
		sendto(sock_fd, "MylinksRegister", sizeof("MylinksRegister") - 1,0,(struct sockaddr*)server_addr,sizeof(struct sockaddr));
		ret = recvfrom(sock_fd, (uint8_t *)tmp, sizeof(tmp) -1, 0,(struct sockaddr*)server_addr,(socklen_t *)&fromlen);
		if(ret >= 0){
			tmp[ret] = '\0';
		}
		if(strlen(tmp)){
			break;
		}
	}
	uart0_sendStr("今日生产幻码为:");
	uart0_sendStr(tmp);
	uart0_sendStr("\r\n");
	magic_id = magic_number_creat(tmp);
	while(!magic_user || update_url == NULL){
        sys_msleep(50);
    }
	if(magic_user!=magic_id){
        uart0_sendStr("***生产密钥输入错误!***\r\n");
        goto err;
    }
	uart0_sendStr("升级路径:");
	uart0_sendStr(update_url);
	uart0_sendStr("\r\n");
	close(sock_fd);
	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == sock_fd){
		uart0_sendStr("\r\n>>>连接失败:-7<<<\r\n");
		goto err;
	}
	memset(tcp_from,0, sizeof(struct sockaddr_in));
	tcp_from->sin_family = AF_INET;
	tcp_from->sin_port = htons(15724);
	tcp_from->sin_addr.s_addr = inet_addr("118.178.87.170");
	numchars = connect(sock_fd,(struct sockaddr *)tcp_from,sizeof(struct sockaddr));
	if( 0!=numchars){	
		uart0_sendStr("\r\n>>>连接失败:-9<<<\r\n");	
		goto err;		
	}
    cJSON* jsRet = NULL;
    char* retStr = NULL;
    jsRet = cJSON_CreateObject();
    if(!jsRet){
        uart0_sendStr("\r\n>>>连接失败:-10<<<\r\n");
        goto err;
    }
	cJSON_AddStringToObject(jsRet,"USER",update_url);
	memcpy(bssid, wlan_get_myaddr(STATION), 6);
	sprintf(tmp,MACSTRS, MAC2STR(bssid));
	uint8_t i;
	for(i = 0; i < strlen(tmp);i++)
    {
        if(tmp[i]>='a'&&tmp[i]<='f')
        {
            tmp[i] = tmp[i] - 'a' + 'A';
        }
    }
    i = 7;
    cJSON_AddStringToObject(jsRet,"MAC",tmp);
    cJSON_AddNumberToObject(jsRet,"SIG",magic_number_creat(tmp));
    cJSON_AddNumberToObject(jsRet,"TYPEID",i);
    retStr = cJSON_PrintUnformatted(jsRet);
    cJSON_Delete(jsRet);
	
	if(write(sock_fd,retStr,strlen(retStr))!=strlen(retStr)){
		uart0_sendStr("\r\n>>>注册失败:-1<<<\r\n");
		goto err;
	}
	
	if(read(sock_fd,&Msg,sizeof(struct message_header))!=sizeof(struct message_header)){
		uart0_sendStr("\r\n>>>注册失败:-2<<<\r\n");
		goto err;		
	}
	if(!memcmp(tmp,Msg.id,12)){
		uart0_sendStr("当前MAC:");
		serial_write(0, tmp, 12);
		uart0_sendStr("\n");
	}else{
		uart0_sendStr("设置MAC:");
		serial_write(0, Msg.id, 12);
		uart0_sendStr("\n");		
	}
	free(retStr);
	retStr = (char *)malloc(256);
	if(retStr == NULL){
		uart0_sendStr("\r\n>>>注册失败:-3<<<\r\n");
		goto err;
	}
	if(update_flag & 0x2){
		uart0_sendStr("\r\n升级文件系统中......");
		sprintf(retStr,"118.178.87.170%s/%s",update_url,"minifs_rom.img");
		if(!Firmware_WIFIOTAByUrl(1,retStr,80)){
			uart0_sendStr("成功!\r\n");
		}else{
			uart0_sendStr("失败!\r\n");
			goto err;
		}	
	}
	if(update_flag & 0x1){
		uart0_sendStr("\r\n升级固件中......");
		sprintf(retStr,"118.178.87.170%s/%s",update_url,"user.img");
		if(!Firmware_WIFIOTAByUrl(0,retStr,80)){
			uart0_sendStr("成功!\r\n");
			if(encryption_save(&Msg) < 0){
				flash_erase(0x60000, 0x40000);
				return;
			}
			uart0_sendStr("\r\n模块重启中...\n");
			reboot(1);
		}else{
			uart0_sendStr("失败!\r\n");
			goto err;
		}	
	}
err:
	close(sock_fd);
	for(;;) sys_msleep(1000);
exit:
    vTaskDelete(NULL);
}


void user_init(void){


	return;
}

