/*******************************************************************************
 * Copyright (c) 2014, 2015 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *    Ian Craggs - convert to FreeRTOS
 *******************************************************************************/

#include "MQTTFreeRTOS.h"


#include "lwip/sockets.h"
#include "lwip/netdb.h"
#if defined(CONFIG_AXTLS) || defined(CONFIG_WOLFSSL)
#include <lynx_dev.h>
#endif

#if 0
#define FREERTOS_SO_RCVTIMEO SO_RCVTIMEO
#define FREERTOS_AF_INET AF_INET
#define FREERTOS_SOCK_STREAM SOCK_STREAM
#define FREERTOS_IPPROTO_TCP IPPROTO_TCP
#define FreeRTOS_closesocket closesocket
#define FreeRTOS_gethostbyname gethostbyname
#define FreeRTOS_htons htons
#define FreeRTOS_socket socket
#define FreeRTOS_connect connect
#define FreeRTOS_setsockopt setsockopt
#define FreeRTOS_send send
#define FreeRTOS_recv recv
#endif

#define printf serial_printf

#if defined(CONFIG_AXTLS) || defined(CONFIG_WOLFSSL)
void *ssl_obj;
extern void *platform_ssl_connect(void *tcp_fd, const char *root_ca, int root_ca_len, const char *client_cert, int client_cert_len, const char *key, int key_len);
extern int platform_ssl_close(void *ssl);
extern int platform_ssl_recv(void *ssl, char *buf, int len);
extern int platform_ssl_send(void *ssl, const char *buf, int len);
#endif

int ThreadStart(Thread* thread, void (*fn)(void*), void* arg)
{
	int rc = 0;
	uint16_t usTaskStackSize = (configMINIMAL_STACK_SIZE * 5);
	UBaseType_t uxTaskPriority = uxTaskPriorityGet(NULL); /* set the priority as the same as the calling task*/

	rc = xTaskCreate(fn,	/* The function that implements the task. */
	                 "MQTTTask",			/* Just a text name for the task to aid debugging. */
	                 usTaskStackSize,	/* The stack size is defined in FreeRTOSIPConfig.h. */
	                 arg,				/* The task parameter, not used in this case. */
	                 uxTaskPriority,		/* The priority assigned to the task is defined in FreeRTOSConfig.h. */
	                 &thread->task);		/* The task handle is not used. */

	return rc;
}


void FreeRTOS_MutexInit(Mutex* mutex)
{
	mutex->sem = xSemaphoreCreateMutex();
}

void FreeRTOS_MutexDeinit(Mutex* mutex)
{
	if (mutex->sem)
		vSemaphoreDelete(mutex->sem);
	mutex->sem = NULL;
}


int FreeRTOS_MutexLock(Mutex* mutex)
{
	return xSemaphoreTake(mutex->sem, portMAX_DELAY);
}

int FreeRTOS_MutexUnlock(Mutex* mutex)
{
	return xSemaphoreGive(mutex->sem);
}


void TimerCountdownMS(Timer* timer, unsigned int timeout_ms)
{
	timer->xTicksToWait = timeout_ms / portTICK_PERIOD_MS; /* convert milliseconds to ticks */
	vTaskSetTimeOutState(&timer->xTimeOut); /* Record the time at which this function was entered. */
}


void TimerCountdown(Timer* timer, unsigned int timeout)
{
	TimerCountdownMS(timer, timeout * 1000);
}


int TimerLeftMS(Timer* timer)
{
	xTaskCheckForTimeOut(&timer->xTimeOut, &timer->xTicksToWait); /* updates xTicksToWait to the number left */
	return (timer->xTicksToWait < 0) ? 0 : (timer->xTicksToWait * portTICK_PERIOD_MS);
}


char TimerIsExpired(Timer* timer)
{
	return xTaskCheckForTimeOut(&timer->xTimeOut, &timer->xTicksToWait) == pdTRUE;
}


void TimerInit(Timer* timer)
{
	timer->xTicksToWait = 0;
	memset(&timer->xTimeOut, '\0', sizeof(timer->xTimeOut));
}


int FreeRTOS_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
	TickType_t xTicksToWait = timeout_ms / portTICK_PERIOD_MS; /* convert milliseconds to ticks */
	TimeOut_t xTimeOut;
	int recvLen = 0;
	int rc = 0;

#if 0
//	printf("%s %d\n", __FILE__, __LINE__);
	FreeRTOS_setsockopt(n->my_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout_ms, sizeof(timeout_ms));
//	printf("%s %d len=%d\n", __FILE__, __LINE__,len);
	recvLen = FreeRTOS_recv(n->my_socket, buffer, len, 0);
//	printf("%s %d read len=%d\n", __FILE__, __LINE__, recvLen);
	return recvLen;
#endif
	vTaskSetTimeOutState(&xTimeOut); /* Record the time at which this function was entered. */
#if defined(CONFIG_AXTLS) || defined(CONFIG_WOLFSSL)
	do
	{
		setsockopt(n->my_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout_ms, sizeof(timeout_ms));
		recvLen = platform_ssl_recv(ssl_obj, buffer, len);
	} while (!recvLen && xTaskCheckForTimeOut(&xTimeOut, &xTicksToWait) == pdFALSE);
#else
	do {
		setsockopt(n->my_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout_ms, sizeof(timeout_ms));
		rc = recv(n->my_socket, buffer + recvLen, len - recvLen, 0);
		if (rc > 0)
			recvLen += rc;
		else if (rc < 0) {
			recvLen = rc;
			break;
		}
	} while (recvLen < len && xTaskCheckForTimeOut(&xTimeOut, &xTicksToWait) == pdFALSE);
#endif

	return recvLen;
}


int FreeRTOS_write(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
	TickType_t xTicksToWait = timeout_ms / portTICK_PERIOD_MS; /* convert milliseconds to ticks */
	TimeOut_t xTimeOut;
	int sentLen = 0;

	vTaskSetTimeOutState(&xTimeOut); /* Record the time at which this function was entered. */
	do {
		int rc = 0;
		setsockopt(n->my_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout_ms, sizeof(timeout_ms));
#if defined(CONFIG_AXTLS) || defined(CONFIG_WOLFSSL)
		rc = platform_ssl_send(ssl_obj, buffer + sentLen, len - sentLen);
#else
		rc = send(n->my_socket, buffer + sentLen, len - sentLen, 0);
#endif

		if (rc > 0)
			sentLen += rc;
		else if (rc < 0) {
			sentLen = rc;
			break;
		}
	} while (sentLen < len && xTaskCheckForTimeOut(&xTimeOut, &xTicksToWait) == pdFALSE);
	return sentLen;
}


void FreeRTOS_closesocket(Network* n)
{
	int ret = -1;
	
	ret = closesocket(n->my_socket);
	printf("closesocket ret=%d\n", ret);
#if defined(CONFIG_AXTLS) || defined(CONFIG_WOLFSSL)
	platform_ssl_close(ssl_obj);
#endif
}


void FreeRTOS_NetworkInit(Network* n)
{
	n->my_socket = 0;
	n->mqttread = FreeRTOS_read;
	n->mqttwrite = FreeRTOS_write;
	n->disconnect = FreeRTOS_closesocket;
}


int FreeRTOS_NetworkConnect(Network* n, char* addr, int port, unsigned char* root_ca, int root_ca_len, unsigned char* client_cert, int client_cert_len, unsigned char* private_key, int private_key_len)
{
	struct sockaddr_in sAddr;
	int ret = -1;
	int keeplive = 500;
	struct hostent *hptr;

	if (isValidIP(addr) == false)
	{
		if ((hptr = (struct hostent *)gethostbyname(addr)) == 0)
		{
			printf("gethostbyname fail\n");
			goto exit;
		}
		sAddr.sin_addr.s_addr = *(u32_t*)hptr->h_addr;
	}
	else
		sAddr.sin_addr.s_addr = inet_addr(addr);/* IP addr, string to int */

	sAddr.sin_port = htons(port);
	sAddr.sin_family = AF_INET;

	if ((n->my_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		printf("socket create fail\n");
		goto exit;
	}

	//keep alive
	//setsockopt(n->my_socket, SOL_SOCKET, SO_KEEPALIVE, (char *) &keeplive,sizeof(int)) ;

	if ((ret = connect(n->my_socket, (struct sockaddr *)&sAddr, sizeof(sAddr))) < 0) {
		printf("socket connect fail\n");
		closesocket(n->my_socket);
		goto exit;
	}
#if defined(CONFIG_AXTLS) || defined(CONFIG_WOLFSSL)
	if((ssl_obj = platform_ssl_connect((void *)n->my_socket, root_ca, root_ca_len, client_cert, client_cert_len, private_key, private_key_len)) != NULL)
		printf("ssl connect success\n");
	else
	{
		printf("ssl connect fail\n");
		ret = -1;
	}
#endif
exit:
	return ret;
}


#if 0
int NetworkConnectTLS(Network *n, char* addr, int port, SlSockSecureFiles_t* certificates, unsigned char sec_method, unsigned int cipher, char server_verify)
{
	SlSockAddrIn_t sAddr;
	int addrSize;
	int retVal;
	unsigned long ipAddress;

	retVal = sl_NetAppDnsGetHostByName(addr, strlen(addr), &ipAddress, AF_INET);
	if (retVal < 0) {
		return -1;
	}

	sAddr.sin_family = AF_INET;
	sAddr.sin_port = sl_Htons((unsigned short)port);
	sAddr.sin_addr.s_addr = sl_Htonl(ipAddress);

	addrSize = sizeof(SlSockAddrIn_t);

	n->my_socket = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, SL_SEC_SOCKET);
	if (n->my_socket < 0) {
		return -1;
	}

	SlSockSecureMethod method;
	method.secureMethod = sec_method;
	retVal = sl_SetSockOpt(n->my_socket, SL_SOL_SOCKET, SL_SO_SECMETHOD, &method, sizeof(method));
	if (retVal < 0) {
		return retVal;
	}

	SlSockSecureMask mask;
	mask.secureMask = cipher;
	retVal = sl_SetSockOpt(n->my_socket, SL_SOL_SOCKET, SL_SO_SECURE_MASK, &mask, sizeof(mask));
	if (retVal < 0) {
		return retVal;
	}

	if (certificates != NULL) {
		retVal = sl_SetSockOpt(n->my_socket, SL_SOL_SOCKET, SL_SO_SECURE_FILES, certificates->secureFiles, sizeof(SlSockSecureFiles_t));
		if (retVal < 0) {
			return retVal;
		}
	}

	retVal = sl_Connect(n->my_socket, (SlSockAddr_t *)&sAddr, addrSize);
	if (retVal < 0) {
		if (server_verify || retVal != -453) {
			sl_Close(n->my_socket);
			return retVal;
		}
	}

	SysTickIntRegister(SysTickIntHandler);
	SysTickPeriodSet(80000);
	SysTickEnable();

	return retVal;
}
#endif
