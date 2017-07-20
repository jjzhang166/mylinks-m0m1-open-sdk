/*=============================================================================+
|                                                                              |
| Copyright 2016                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file omniconfig.h 
*   \brief define omniconfig result and interface functions 
*   \author Montage
*/

#ifndef OMNICONFIG_H
#define OMNICONFIG_H


/*=============================================================================+
| Define                                                                       |
+=============================================================================*/

/*OMNICONFIG Result*/
#define OMNI_RET_SUCCESS			0 /* Success */
#define OMNI_RET_TIMEOUT			1 /* use time over request*/
#define OMNI_RET_ERR_SSID			2 /* SSID LENGTH > 32 .*/
#define OMNI_RET_ERR_PWD			3 /* PWD LENGTH > 64 .*/
#define OMNI_RET_ERR_UNKNOW			4 /* omnicfg internal error */


/*Send UDP Message to OMNICONFIG APP*/

/*STATE*/
#define OMNI_STATE_NONE							0/* Initial*/
#define OMNI_STATE_CONNECTING					1/* START */
#define OMNI_STATE_CONNECT_SUCCESS				2/* Result OK:Successfully applied*/
/*Error Msg*/
#define OMNI_STATE_AP_NOT_FOUND				3/* SCAN:Target AP not found*/
#define OMNI_STATE_PASSWORD_ERR				4/* Wrong Password of target AP*/
#define OMNI_STATE_NO_IP_GOT					5/* DHCP Fail:Cannot get IP*/
#define OMNI_STATE_IP_CONNECTIVITY_TEST_FAILED	6/* PING FAIL:Failed to test IP connectivity*/
#define OMNI_STATE_CONNECT_TIMEOUT				7/* Timeout to connect AP*/

/*-----------------------------------------------------------------------------+
| Function Prototypes                                                          |
+-----------------------------------------------------------------------------*/

/* OMINCONFIG Callback function*/
typedef void (*omniconfig_callback)(char *ssid, char *key, int mode, int error);

/* Register OMNICONFIG Callback function */
/* timeout_ms min value as 10000 (10 sec) ; default value as 120000 (120 sec) when set NULL */
void * omnicfg_siso2_start( omniconfig_callback omni_cb,int timeout_ms);

/* User could use the STOP API to stop OMNICONFIG progress*/
void omnicfg_siso2_stop(void);

/*Send UDP Message to OMNICONFIG APP*/
int omnicfg_broadcast(int result,char* ipaddr,char* mac_str,int new);

#endif /*OMNICONFIG_H*/
