/*=============================================================================+
|                                                                              |
| Copyright 2015                                                             |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file app_init.c
*   \brief main entry
*   \author Montage
*/

/*=============================================================================+
| Included Files                                                               |
+=============================================================================*/
#include <c_types.h>
#include <stdint.h>
#include <event.h>
#include <common.h>
#include <os_api.h>
#include <net_api.h>
#include <wla_api.h>
#include <cfg_api_new.h>
#include <gpio.h>
#include <version.h>
#include <built_info.h>
#include <omniconfig.h>

#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#if defined(CONFIG_LWIP)
#include <net_api.h>
#endif
#include <user_config.h>
#include <mylinks_wifi.h>
#include <lwip/sockets.h>
/*=============================================================================+
| Define                                                                       |
+=============================================================================*/



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
	return PT_EXITED;
}


static char *testStr = "Only Test";




static void  udp_demo( void *arg )
{
	int fd = -1;
	int len;
	int ret = 0;
	uint8_t *buf = NULL;
	//1 s超时收数据
	int nNetTimeout = 1000;
	struct sockaddr_in *send_addr = NULL;
	struct sockaddr_in *local_addr = NULL;
	socklen_t fromlen = sizeof(struct sockaddr_in);
	buf = (uint8_t *)malloc(256);
	//等待WIFI模块连接路由器成功
	while(get_slinkup() != STA_LINK_GET_IP){
		sys_msleep(50);
	}
	send_addr = (struct sockaddr_in *)zalloc(sizeof(struct sockaddr_in));
	local_addr = (struct sockaddr_in *)zalloc(sizeof(struct sockaddr_in));

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	//绑定本地的端口为1000
	memset(local_addr, 0, sizeof(struct sockaddr_in));
	local_addr->sin_family = AF_INET;
	local_addr->sin_port = htons(1000);
	local_addr->sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr->sin_len = sizeof(struct sockaddr_in);
	ret = bind(fd, (struct sockaddr *)local_addr, sizeof(struct sockaddr_in));
	if(ret != 0){
		//绑定端口号失败
		goto exit;
	}
	for(;;){
		//设置超时时间为1秒的接收
		setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));
		len = recvfrom(fd, (uint8_t *)buf, 256, 0,(struct sockaddr*)send_addr,(socklen_t *)&fromlen);
		//接收数据长度大于0时，接收到UDP数据
		//此时结构体中将记录下发送到本模块IP和端口号的数据来源，即数据发送过来的IP和端口号
		if(len > 0){
			uart0_tx_buffer(buf,len);
		}else{
			continue;
		}
		
		//以下为发送数据目标地址和端口设置：当要把数据直接返回给之前的IP和地址时，以下不用设置
		/********************************************************/
		//本地UDP往192.168.1.100:1000定向发送
		memset(send_addr, 0, sizeof(struct sockaddr_in));
		send_addr->sin_family = AF_INET;
		send_addr->sin_addr.s_addr = inet_addr("192.168.1.1000");
		send_addr->sin_port = htons(1000);
		send_addr->sin_len = sizeof(struct sockaddr_in);
		/********************************************************/
		
		len = sendto(fd, testStr, strlen(testStr),0,(struct sockaddr*)send_addr,sizeof(struct sockaddr));
		if(len == strlen(testStr)){
			//数据发送成功
		}
	}
exit:
    vTaskDelete(NULL);
	return;
}


void uart_init(void)
{
	/*
	设置串口参数：
	参数1->波特率：115200
	参数2->校验位：无
	参数3->停止位：1位
	参数4->串口号：0号串口
	*/
	serial_conf(baudrate_select(115200), 0, 1, 0,0 );
	serial_init(0);

}


void test_uart0_rev( void * arg){
	uint8_t temp;
	extern struct serial_buffer *ur0_rxbuf;
	for(;;){
		//等待串口是否有数据传入
		if(0 != uart_recv_sem_wait(portMAX_DELAY)){
			continue;
		}
		//判断串口数据是否为空
		while(serial_buffer_empty(ur0_rxbuf)){
			//读取一个字节的串口数据
			temp = serial_buffer_getchar(ur0_rxbuf);
		}
	}
}

#define STA_SSID "Mylinks"
#define STA_PWD "welcometomylinks"

#define AP_SSID "AP_Test"
#define AP_PWD "TEST123456"



void user_init(void){
	struct station_config sta;
	struct softap_config ap;
	uart_init();
	//注册一个串口0的接收任务进行数据接收
	uart0_rev_register(test_uart0_rev);
	//设置模块为AP+STA工作模式
	//wifi_set_opmode(OPMODE_STA);
	wifi_set_opmode(OPMODE_APSTA);
	//读取当前模块的STA配置信息
	wifi_station_set_config(&sta);
	if(strcmp(sta.ssid,STA_SSID) ||
		strcmp(sta.password,STA_PWD)){

		memset(&sta,0,sizeof(sta));
		//设置连接的路由器ssid
		strcpy(sta.ssid,STA_SSID);
		//设置连接的路由器密码
		strcpy(sta.password,STA_PWD);
		wifi_station_set_config(&sta);
	}
	wifi_softap_get_config(&ap);
	memset(ap.ssid,0,32);
	//设置连接的AP的ssid
	strcpy(ap.ssid,AP_SSID);
	//设置连接的AP的密码
	memset(ap.password,0,64);
	//如果不需要密码，以下不用设置
	strcpy(ap.password,AP_PWD);
	wifi_softap_set_config(&ap);
	xTaskCreate(udp_demo, "udp_demo", TASK_HEAP_LEN, 0, 5, NULL);
	return;
}



