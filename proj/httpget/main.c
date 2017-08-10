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
#define STA_SSID "Mylinks"
#define STA_PWD "welcometomylinks"


#define SOCKET_RECBUFFER_LEN 1024


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




static void  httpclient( void *arg )
{
    int32_t ret = 0;
    int32_t http_socketid = -1;
    int8_t http_ip[16]={0};
    int8_t *url = NULL;
    int8_t *host = NULL;  
    uint8 *httpReceiveBuf = NULL;
    //等待模块连接到路由器 
    while(get_slinkup() != STA_LINK_GET_IP){
    	sys_msleep(50);
	}
	//解析url地址
	if(0 > Http_GetHost( "http://www.baidu.com/", &host, &url ) )
    {
    	uart0_sendStr("Http_GetHost failed!\n");
        goto exit;
    }
    //将域名解析为IP地址
	if(isValidIP(host) == false){
		net_get_hostbyname((const char*)host,http_ip);
	}else{
		memcpy(http_ip, host,16);
	}
	//连接服务器
	http_socketid = initSocket(http_ip, 80, 0 );
	if(http_socketid < 0)
	{
	    if(NULL!=host)
	    {
	       free(host);
	       host = NULL;
	    }
	    if(NULL!=url)
	    {
	       free(url);
	       url = NULL;
	    }
		uart0_sendStr("initSocket error\r\n");
        goto exit;
	}
	//发送http头数据
	ret = Http_ReqGetSendHead( url, host, http_socketid );
	if( ret == 0){
		httpReceiveBuf = (uint8 *)malloc(SOCKET_RECBUFFER_LEN);
		if(httpReceiveBuf == NULL){
			goto exit;
		}
		int timeout=2000;//5s timeout
		int rclen = 0;
		int receiveLength = 0;
		//设置接收数据超时时间为2秒
		setsockopt(http_socketid,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(int));
		while(receiveLength < SOCKET_RECBUFFER_LEN)
		{
			rclen = Http_ReadSocket( http_socketid, httpReceiveBuf + receiveLength, SOCKET_RECBUFFER_LEN - receiveLength);
			if(rclen <= 0)
			{
				break;
			}
			//serial_printf("Http_ResGetFirmware len %d\n", rclen);
			receiveLength += rclen;
		}
		ret = Http_Response_Code(httpReceiveBuf);
		if((200 != ret) && (206 != ret))
		{
			uart0_sendStr("Http_Response_Code Err\n\n");
			goto fail;
		}
		uart0_sendStr("Http_Response OK\r\n");
		int headlen = 0;
		//获取HTTP的头长度
		headlen = Http_HeadLen(httpReceiveBuf);
		if(0 == headlen){
			goto fail;
		}
		//将http的头打印出来
		uart0_sendStr("Http Head:");
		uart0_tx_buffer(httpReceiveBuf,headlen);

		//计算出第一次读取buff中多余的数据部分

		uart0_sendStr("Http Body:");
		if((receiveLength - headlen) > 0){
			uart0_tx_buffer(httpReceiveBuf + headlen,receiveLength - headlen);
		}
		int32 filelen = 0;
		//获取body部分的数据
		filelen = Http_BodyLen(httpReceiveBuf);
		receiveLength = 0;
		//读出剩余的body部分的数据
		while(receiveLength < filelen){
			rclen = filelen - receiveLength;
			rclen = (rclen > SOCKET_RECBUFFER_LEN)?SOCKET_RECBUFFER_LEN:rclen;
			rclen = Http_ReadSocket( http_socketid, httpReceiveBuf, rclen);
			if(rclen <= 0){
				break;
			}
			receiveLength +=rclen;
			uart0_tx_buffer(httpReceiveBuf,rclen);
		}

	}
fail:
	free(httpReceiveBuf);
	close(http_socketid);
exit:
	Http_GetFinish();
    sys_msleep(portMAX_DELAY);
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





void user_init(void){
	struct station_config sta;
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
	xTaskCreate(httpclient, "client", TASK_HEAP_LEN, 0, 5, NULL);
	return;
}



