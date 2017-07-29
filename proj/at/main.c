/*=============================================================================+
|                                                                              |
| Copyright 2017                                                               |
| Mylinks Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file at_command_demo.c
*   \brief main entry
*   \author Mylinks
*/

/*=============================================================================+
| Included Files                                                               |
+=============================================================================*/
#include <stdint.h>
#include <common.h>
#include <flash_api.h>
#include <lynx_debug.h>
#include <gpio.h>
#include <serial.h>
#include <event.h>
#include <os_api.h>
#include <net_api.h>
#include <wla_api.h>
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
#include <webserver/webserver.h>

#define UART0   0//Lynx UART1

extern const php_funcationType php_fun[];
extern const php_funPostType post_php_fun[];
extern at_funType at_test[];

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
	serial_conf(baudrate_select(g_atParam.ur_param.baudrate), g_atParam.ur_param.parity, g_atParam.ur_param.stopbits, UART0,threshold );
	serial_init((int)UART0);

#ifdef CONFIG_UR_FLOW_CONTROL
	pin_mode(CONFIG_UR_CTS, 0);
	digital_write(CONFIG_UR_RTS, 0);//RTS active
	pin_en_intr(CONFIG_UR_CTS, 0, (gpio_func_cb)uart_flow_cts_stat, NULL);//0: rising
	pin_en_intr(CONFIG_UR_CTS, 1, (gpio_func_cb)uart_flow_cts_stat, NULL);//1: falling
	pin_mode(CONFIG_UR_RTS, 1);
#endif	
}


void user_init(void){
	//串口设置功能
	uart_init();
	//at_test:用户也自定义at指令
	at_cmd_init(at_test);
	//挂载mini文件系统
	if(mfmount(FS1_FLASH_ADDR) == RES_OK){
		
		//设置web服务器的登录账号
		//注意：长度<=31
		set_web_login("login");
		//设置web服务器的密码
		//注意：长度<=31
		set_web_pwd("password");
		/*如果设置web服务器登录无账号则为：
			set_web_login("");
			set_web_pwd("");
		*/
		//当连接AP时，可以通过下面的URL来打开内置网页
		dnsserver_init("set.mqlinks.com");
		//启动内置web服务器
		webserver_init(php_fun,post_php_fun);
	}
	return;
}

