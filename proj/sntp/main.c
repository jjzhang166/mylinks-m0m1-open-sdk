/*=============================================================================+
|                                                                              |
| Copyright 2017                                                               |
| Mylinks Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file app_init.c
*   \brief main entry
*   \author Mylinks
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
#include <mylinks_sntp.h>
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

uint8_t tbuf[1024];


static void  sntp_demo( void *arg )
{
	struct tm * t;

	//等待WIFI模块连接路由器成功
	while(get_slinkup() != STA_LINK_GET_IP){
		sys_msleep(50);
	}
	while(sntp_init("cn.pool.ntp.org") != 0){
		sys_msleep(100);
	}
	uart0_sendStr("get sntp\r\n");

	for(;;){
		//获取东八区
		t = get_local_time(8);
		sprintf(tbuf,"%d-%d-%d %d:%d:%d\r\n",t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
		uart0_sendStr(tbuf);
		sys_msleep(1000);
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




void user_init(void){
	struct station_config sta;
	uart_init();
	//注册一个串口0的接收任务进行数据接收
	uart0_rev_register(test_uart0_rev);
	//设置模块为STA工作模式
	wifi_set_opmode(OPMODE_STA);

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
	xTaskCreate(sntp_demo, "sntp_demo", TASK_HEAP_LEN, 0, 5, NULL);
	return;
}



