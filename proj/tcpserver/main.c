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
#include <socket_api.h>
/*=============================================================================+
| Define                                                                       |
+=============================================================================*/


extern struct net_dev *idev;
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



static void  tcpserver( void *arg )
{
	int i = 0;
	fd_set fdsr;   
    struct timeval tv; 
    int maxsock = -1;  
	int ret = 0;
	int len;
	socklen_t socklen;
	socklen = sizeof(struct sockaddr_in);
	for(;;){
		sys_msleep(20);
		//清空监控标志位
		FD_ZERO(&fdsr);
		maxsock = -1; 
		//设置read超时最大时间为1秒
		tv.tv_sec = 1;  
		tv.tv_usec = 0;
		//取出5路中最大的fd数值
		for (i = 0; i < 5; i++){
			if (idev->s[i] == -1)  
			{  
				continue;
			}
			FD_SET(idev->s[i], &fdsr);
			if(maxsock < idev->s[i]){
				maxsock = idev->s[i];
			}

		}
		if(maxsock == -1){
			continue;
		}
		
		//开始超时监控
		ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv); 

		if (ret <= 0)  
		{
			continue;  
		} 
		for (i = 0; i < 5; i++) 
		{
			//如果当前fd存在，并且有数据生产，则读取数据。
			if (idev->s[i] != -1 && FD_ISSET(idev->s[i], &fdsr))  
			{  
				len = recvfrom(idev->s[i], idev->trans_txb, SOCKET_LEN, 0,
					(struct sockaddr *)&idev->fromaddr[i], &socklen);
				//如果数据读取失败，则关半当前的tcp 连接
				if (len <= 0) 
				{
					net_socket_del(i);
					continue;
				}
				//从串口输出接收到的数据
				uart0_tx_buffer(idev->trans_txb,len);
				//往当时tcp发送一个数据
				len = sendto(idev->s[i], testStr, strlen(testStr), 0,(struct sockaddr *)&idev->fromaddr[i], socklen);
				
				if(strlen(testStr) == len){
					//发送成功
				}else{
					//异常处理
					net_socket_del(i);
				}
			}
		}
	}

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

int server_port = 23;


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
	//建立TCP连接进程，最多连接入5路的tcp client
	tcpserver_start(&server_port);
	xTaskCreate(tcpserver, "server", TASK_HEAP_LEN, 0, 5, NULL);
	return;
}



