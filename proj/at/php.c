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
/*
	本文件中定义了webserver两个实例
	请结合劢领智能开源的html5和js进行分析
*/
#include <lib_autoconf.h>
#include <c_types.h>
#include <socket_api.h>
#include <cfg_api_new.h>
#include <user_config.h>
#include <webserver/webserver.h>
#include <cJSON/cJSON.h>
#include <user_config.h>
#include <version.h>
#include <wla_api.h>

static int8_t get_systeminfo(int client);
static int8_t set_wifisystem(int client,webConn *conn);


/*
	定义自定义的get通信结构体数组
	本例子中，用户往浏览器get /system/status.php时，将会进入get_systeminfo()函数进行
	此时用户可以在get_systeminfo()函数中作相应的处理
*/
const php_funcationType php_fun[]={
	{"status.php", 10, get_systeminfo},
	{NULL,0,not_found},
};



/*
	定义自定义的get通信结构体数组
	本例子中，用户往浏览器post /system/set.php时，将会进入set_wifisystem()函数进行
	此时用户可以在get_systeminfo()函数中作相应的处理
*/
const php_funPostType post_php_fun[]={
	{"set.php", 7, set_wifisystem},
	{NULL,0,post_not_found},
};




static int8_t get_systeminfo(int client){
	cJSON* jsRet = NULL;
	char *buf,*retStr;
	link_sts linkStatus;
	ip_sts outNetpara;
	uint8_t bssid[6];
	buf = (char *)malloc(CONTENT_LEN);
	if(NULL == buf){
		return -1;
	}
	jsRet = cJSON_CreateObject();
	if(NULL == jsRet){
		free(buf);
		return -1;
	}
	sprintf(buf,"%s."LVER"",sw_build_sdk);
	//版本系统
	cJSON_AddStringToObject(jsRet,"ver",buf);
	//工作模式
	cJSON_AddNumberToObject(jsRet,"mode",g_rfParam.work_mode);
	//AP的SSID
	cJSON_AddStringToObject(jsRet,"assid",g_rfParam.ap_param.ssid);
	//AP的IP地址
	cJSON_AddStringToObject(jsRet,"aip",g_atParam.apip_param.ip);
	//AP的MAC
	memcpy(bssid, wlan_get_myaddr(SOFT_AP), 6);
	sprintf(buf,MACSTRS, MAC2STR(bssid));
	cJSON_AddStringToObject(jsRet,"amac",buf);

	//STA的SSID
	cJSON_AddStringToObject(jsRet,"ssid",g_rfParam.sta_param.ssid);
	//连接状态
	wlan_get_link_sts(&linkStatus, STATION);
	cJSON_AddNumberToObject(jsRet,"stats",linkStatus.is_connected);
	//信号强度
	cJSON_AddNumberToObject(jsRet,"signal",linkStatus.wifi_strength);
	//分配到的IP地址
	net_if_ip_sts(&outNetpara, STATION);
	cJSON_AddStringToObject(jsRet,"ip",outNetpara.ip);

	//MAC地址
	memcpy(bssid, wlan_get_myaddr(STATION), 6);
	sprintf(buf,MACSTRS, MAC2STR(bssid));
	cJSON_AddStringToObject(jsRet,"mac",buf);
	//AP设置的通道
	cJSON_AddNumberToObject(jsRet,"wchn",g_rfParam.ap_param.channel);
	//AP的密码
	cJSON_AddStringToObject(jsRet,"iwep",g_rfParam.ap_param.key);

	//设置AP的IP
	cJSON_AddStringToObject(jsRet,"amask",g_atParam.apip_param.mask);
	//设置AP的DHCP服务器状态
	cJSON_AddNumberToObject(jsRet,"adhcp",!!(g_atParam.dhcp_mode & 0x1));
	//设置STA的密码
	cJSON_AddStringToObject(jsRet,"swep",g_rfParam.sta_param.key);
	cJSON_AddNumberToObject(jsRet,"sdhcp",!!(g_atParam.dhcp_mode & 0x2));
	cJSON_AddStringToObject(jsRet,"smask",outNetpara.mask);
	cJSON_AddStringToObject(jsRet,"sgw",outNetpara.gate);
	cJSON_AddStringToObject(jsRet,"sdns",outNetpara.dns);

	cJSON_AddNumberToObject(jsRet,"baud",baudrate_select(g_atParam.ur_param.baudrate));
	cJSON_AddNumberToObject(jsRet,"stop",g_atParam.ur_param.stopbits);
	cJSON_AddNumberToObject(jsRet,"par",g_atParam.ur_param.parity);

	cJSON_AddStringToObject(jsRet,"sserver",g_atParam.socka.ip);
	cJSON_AddNumberToObject(jsRet,"type",g_atParam.socka.type & 0x3);
	cJSON_AddNumberToObject(jsRet,"port",g_atParam.socka.port);
	cJSON_AddNumberToObject(jsRet,"timeout",g_atParam.socka.time_alive);
	cJSON_AddStringToObject(jsRet,"wuser",g_atParam.webkeyParam.name);
	cJSON_AddStringToObject(jsRet,"wkey",g_atParam.webkeyParam.key);
	retStr = cJSON_PrintUnformatted(jsRet);
	cJSON_Delete(jsRet);
	http_head(client,strlen(retStr));
	STRSEND(client,retStr);
	free(retStr);
	free(buf);
	return 0;
}




static int8_t set_wifisystem(int client,webConn *conn){
	char *buf = NULL;
	int32_t len;
	cJSON *Json,*s;
	if(conn->postLen <= 0){
		return;
	}
	buf = (char *)malloc(conn->postLen + 1);
	len = read(client,buf,conn->postLen);
	if(len != conn->postLen){
		goto SETOUT;
	}
	buf[len] = '\0';
	Json = cJSON_Parse(buf);
	if(Json == NULL){
		goto SETOUT;
	}
	s = cJSON_GetObjectItem(Json, "apname");
	//设置AP名称
	if(s != NULL){
		len = strlen(s->value.valuestring);
		if( len > 32 ) len = 32;
		memcpy(g_rfParam.ap_param.ssid,s->value.valuestring,len);
		g_rfParam.ap_param.ssid[len] = '\0';
	}
	//设置通道号
	s = cJSON_GetObjectItem(Json,"apchn");
	if(s != NULL){
		len = atoi(s->value.valuestring);
		if(len > 13) len = 13;
		else if(len < 1) len = 1;
		g_rfParam.ap_param.channel = len;
	}
	//设置AP的密码
	s = cJSON_GetObjectItem(Json, "apkey");
	if(s != NULL){
		len = strlen(s->value.valuestring);
		if( len > 64 ) len = 64;
		memcpy(g_rfParam.ap_param.key,s->value.valuestring,len);
		g_rfParam.ap_param.key[len] = '\0';
	}
	s = cJSON_GetObjectItem(Json, "apip");
	if(s != NULL){
		strcpy(g_atParam.apip_param.ip,s->value.valuestring);
		strcpy(g_atParam.apip_param.gw,s->value.valuestring);
	}
	s = cJSON_GetObjectItem(Json, "apmask");
	if(s != NULL){
		strcpy(g_atParam.apip_param.mask,s->value.valuestring);
	}
	s = cJSON_GetObjectItem(Json, "apdhcp");
	if(s != NULL){
		if(atoi(s->value.valuestring) == 1){
			g_atParam.dhcp_mode |= 0x1;
		}else{
			g_atParam.dhcp_mode &= (~0x1);
		}
	}
	s = cJSON_GetObjectItem(Json, "staname");
	if(s != NULL){
		len = strlen(s->value.valuestring);
		if( len > 32 ) len = 32;
		memcpy(g_rfParam.sta_param.ssid,s->value.valuestring,len);
		g_rfParam.sta_param.ssid[len] = '\0';
		g_rfParam.sta_param.channel = 0;
		g_rfParam.sta_param.enc = SECURITY_AUTO;
		memset(g_rfParam.sta_param.bssid,0,6);
	}
	s = cJSON_GetObjectItem(Json,"stakey");
	if(s != NULL){
		len = strlen(s->value.valuestring);
		if( len > 64 ) len = 64;
		memcpy(g_rfParam.sta_param.key,s->value.valuestring,len);
		g_rfParam.sta_param.key[len] = '\0';
		g_rfParam.sta_param.channel = 0;
		g_rfParam.sta_param.enc = SECURITY_AUTO;
		memset(g_rfParam.sta_param.bssid,0,6);
	}
	s = cJSON_GetObjectItem(Json,"stadhcp");
	if(s != NULL){
		if(atoi(s->value.valuestring) == 1){
			g_atParam.dhcp_mode |= 0x2;
		}else{
			g_atParam.dhcp_mode &= (~0x2);
		}
	}
	s = cJSON_GetObjectItem(Json,"staip");
	if(s != NULL){
		strcpy(g_atParam.staip_param.ip,s->value.valuestring);
	}
	s = cJSON_GetObjectItem(Json,"stamask");
	if(s != NULL){
		strcpy(g_atParam.staip_param.mask,s->value.valuestring);
	}
	s = cJSON_GetObjectItem(Json,"stagw");
	if(s != NULL){
		strcpy(g_atParam.staip_param.gw,s->value.valuestring);
	}
	//协议
	s = cJSON_GetObjectItem(Json,"type");
	if(s != NULL){
		len = atoi(s->value.valuestring);
		if(len > 3) len = 3;
		else if(len < 0) len = 0;
		g_atParam.socka.type = len;
	}
	s = cJSON_GetObjectItem(Json,"server");
	if(s != NULL){
		strcpy(g_atParam.socka.ip,s->value.valuestring);
	}
	s = cJSON_GetObjectItem(Json,"port");
	if(s != NULL){
		g_atParam.socka.port = atoi(s->value.valuestring);
	}
	s = cJSON_GetObjectItem(Json,"timeout");
	if(s != NULL){
		g_atParam.socka.time_alive = atoi(s->value.valuestring);
	}
	s = cJSON_GetObjectItem(Json,"baud");
	if(s != NULL){
		g_atParam.ur_param.baudrate = atoi(s->value.valuestring);
	}
	s = cJSON_GetObjectItem(Json,"stop");
	if(s != NULL){
		g_atParam.ur_param.stopbits = atoi(s->value.valuestring);
	}
	s = cJSON_GetObjectItem(Json,"par");	
	if(s != NULL){
		g_atParam.ur_param.parity = atoi(s->value.valuestring);
	}
	s = cJSON_GetObjectItem(Json,"Nuser");
	if(s != NULL){
		len = strlen(s->value.valuestring);
		if( len > 31) len = 31;
		memcpy(g_atParam.webkeyParam.name,s->value.valuestring,len);
		g_atParam.webkeyParam.name[len] = '\0';
	}
	s = cJSON_GetObjectItem(Json,"Nkey");
	if(s != NULL){
		len = strlen(s->value.valuestring);
		if( len > 31) len = 31;
		memcpy(g_atParam.webkeyParam.key,s->value.valuestring,len);
		g_atParam.webkeyParam.key[len] = '\0';
	}
	s = cJSON_GetObjectItem(Json,"mode");
	if(s != NULL){
		len = atoi(s->value.valuestring);
		if(len > 2) len = 2;
		else if(len < 0) len = 0;
		g_rfParam.work_mode = len;
	}
	s = cJSON_GetObjectItem(Json,"active");
	if(s != NULL){
		if(s->value.valueint & 0x4){
			config_erase();
			rf_config_erase();
		}
		if(s->value.valueint & 0x2){
			config_submit();
			rf_config_submit();
		}	
		if(s->value.valueint & 0x1){
			reboot(1);
		}			
	}
	cJSON_Delete(Json);
	http_head(client,strlen("OK"));
	STRSEND(client,"OK");
SETOUT:
	free(buf);
	return 0;
}





