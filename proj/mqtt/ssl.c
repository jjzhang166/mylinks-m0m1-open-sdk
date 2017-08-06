/*
 * Copyright (c) 2014-2015 Alibaba Group. All rights reserved.
 *
 * Alibaba Group retains all right, title and interest (including all
 * intellectual property rights) in and to this computer program, which is
 * protected by applicable intellectual property laws.  Unless you have
 * obtained a separate written license from Alibaba Group., you are not
 * authorized to utilize all or a part of this computer program for any
 * purpose (including reproduction, distribution, modification, and
 * compilation into object code), and you must immediately destroy or
 * return to Alibaba Group all copies of this computer program.  If you
 * are licensed by Alibaba Group, your rights to utilize this computer
 * program are limited by the terms of that license.  To obtain a license,
 * please contact Alibaba Group.
 *
 * This computer program contains trade secrets owned by Alibaba Group.
 * and, unless unauthorized by Alibaba Group in writing, you agree to
 * maintain the confidentiality of this computer program and related
 * information and to not disclose this computer program and related
 * information to any other person or entity.
 *
 * THIS COMPUTER PROGRAM IS PROVIDED AS IS WITHOUT ANY WARRANTIES, AND
 * Alibaba Group EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED,
 * INCLUDING THE WARRANTIES OF MERCHANTIBILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, TITLE, AND NONINFRINGEMENT.
 */

#include <lynx_dev.h>
#include <event.h>
#include <common.h>
#include <lwip/sockets.h>
#include <lynx_debug.h>
#if defined(CONFIG_FREERTOS)
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#endif
#if defined(CONFIG_AXTLS)
#include <ssl.h>
#elif defined(CONFIG_WOLFSSL)
#include <wolfssl/ssl.h>
#endif


#if defined(CONFIG_AXTLS)
#if defined(CONFIG_FREERTOS)
static void *platform_mutex_init(void)
{
	return (void*)xSemaphoreCreateMutex();
}

static void platform_mutex_lock(void *mutex)
{
	if (mutex != NULL)
		xSemaphoreTake(mutex, portMAX_DELAY);
}

static void platform_mutex_unlock(void *mutex)
{
	if (mutex != NULL)
		xSemaphoreGive(mutex);
}

static void platform_mutex_destroy(void *mutex)
{
	if (mutex != NULL)
		vSemaphoreDelete(mutex);
}
#endif

static SSL_CTX *ssl_ctx = NULL;
static void* s_xSemaphore = NULL;

void *platform_ssl_connect(void *tcp_fd, const char *root_ca, int root_ca_len, const char *client_cert, int client_cert_len, const char *key, int key_len)
{
	SSL *ssl_temp;

	if (!ssl_ctx)
    {
        ssl_ctx = ssl_ctx_new(SSL_DISPLAY_CERTS|SSL_NO_DEFAULT_KEY|SSL_DISPLAY_RSA|SSL_DISPLAY_STATES|SSL_DISPLAY_BYTES, SSL_DEFAULT_CLNT_SESS);
        if (!ssl_ctx)
        {
            DBG_PRINTF(ERROR,"fail to initialize ssl context \n");
            return NULL;
        }
		if(ssl_obj_load1(ssl_ctx, SSL_OBJ_X509_CACERT, root_ca, root_ca_len, NULL))
		{
            DBG_PRINTF(ERROR,"ssl obj load fail\n");
			return NULL;
		}
		if(ssl_obj_load1(ssl_ctx, SSL_OBJ_X509_CERT, client_cert, client_cert_len, NULL))
		{
            DBG_PRINTF(ERROR,"ssl obj load fail\n");
			return NULL;
		}
		if(ssl_obj_load1(ssl_ctx, SSL_OBJ_RSA_KEY, key, key_len, NULL))
		{
            DBG_PRINTF(ERROR,"ssl obj load fail\n");
			return NULL;
		}
	}
    else
    {
        DBG_PRINTF(INFO,"ssl context already initialized \n");
    }

	ssl_temp = ssl_client_new(ssl_ctx, (int)tcp_fd, NULL, 0, NULL);

	if (ssl_handshake_status(ssl_temp) != SSL_OK)
    {
        DBG_PRINTF(ERROR,"failed create ssl connection \n");
        goto err;
    }
	s_xSemaphore = (void*)platform_mutex_init();
	if(s_xSemaphore == NULL)
	{
		DBG_PRINTF(INFO,"ssl xSemaphoreCreateMutex FAILED \n");
		goto err;
	}
    return ssl_temp;
err:
	if (ssl_temp)
		ssl_free(ssl_temp);

	return NULL;
}



int platform_ssl_close(void *ssl)
{
	platform_mutex_destroy(s_xSemaphore);
	s_xSemaphore = NULL;

    ssl_free((SSL*)ssl);

    if (ssl_ctx)
    {
        ssl_ctx_free(ssl_ctx);
        ssl_ctx = NULL;
    }

    return 0;
}



int platform_ssl_recv(void *ssl, char *buf, int len)
{
	uint8_t *read_buf;
    int ret;

#if 1
	//while (ret == SSL_OK)
	{
		platform_mutex_lock(s_xSemaphore);
		ret = ssl_read(ssl, &read_buf);
		if (ret > SSL_OK)
		{
			memcpy(buf, read_buf, ret > len ? len : ret);
		}
		platform_mutex_unlock(s_xSemaphore);
	}
#else
	platform_mutex_lock(s_xSemaphore);
    while ((ret = ssl_read(ssl, &read_buf)) == SSL_OK);

    if (ret > SSL_OK)
    {
        memcpy(buf, read_buf, ret > len ? len : ret);
    }
	platform_mutex_unlock(s_xSemaphore);
#endif

    return (ret > len ? len : ret);
}

int platform_ssl_send(void *ssl, const char *buf, int len)
{
	platform_mutex_lock(s_xSemaphore);
    int ret = ssl_write((SSL*)ssl, buf, len);
	platform_mutex_unlock(s_xSemaphore);

    return (ret);
}
#elif defined(CONFIG_WOLFSSL)
static WOLFSSL_CTX* ssl_ctx = NULL;
//static X509_STORE *ca_store = NULL;
//static X509 *ca = NULL;


static int ssl_init( const char *my_ca )
{
    if (!ssl_ctx)
    {
        WOLFSSL_METHOD *meth;

        meth = wolfTLSv1_2_client_method();

        ssl_ctx = wolfSSL_CTX_new(meth);
        if (!ssl_ctx)
        {
            DBG_PRINTF(ERROR,"fail to initialize ssl context \n");
            return -1;
        }
	}
    else
    {
        DBG_PRINTF(INFO,"ssl context already initialized \n");
    }

    return 0;
}



static int ssl_establish(int sock, WOLFSSL **ppssl)
{
    int err;
    WOLFSSL *ssl_temp;

    if (!ssl_ctx)
    {
        DBG_PRINTF(ERROR,"no ssl context to create ssl connection \n");
        return -1;
    }

    ssl_temp = wolfSSL_new(ssl_ctx);

    wolfSSL_set_fd(ssl_temp, sock);
    err = wolfSSL_connect(ssl_temp);

    if (err == -1)
    {
        DBG_PRINTF(ERROR,"failed create ssl connection \n");
        goto err;
    }

    *ppssl = (void *)ssl_temp;

    return 0;

err:
    if (ssl_temp)
    {
        wolfSSL_free(ssl_temp);
    }
 
    *ppssl = NULL;
    return -1;
}



void *platform_ssl_connect(void *tcp_fd, const char *root_ca, int root_ca_len, const char *client_cert, int client_cert_len, const char *key, int key_len)
{
    WOLFSSL *pssl;
    if (0 != ssl_init(root_ca))
    {
        return NULL;
    }
    if (0 != ssl_init(client_cert))
    {
        return NULL;
    }
    if (0 != ssl_init(key))
    {
        return NULL;
    }

	if (wolfSSL_CTX_load_verify_buffer(ssl_ctx, root_ca, root_ca_len, SSL_FILETYPE_PEM) != SSL_SUCCESS)
	{
		DBG_PRINTF(ERROR, "can't load buffer ca file");
		return NULL;
	}
	else if (wolfSSL_CTX_use_certificate_buffer(ssl_ctx, client_cert, client_cert_len, SSL_FILETYPE_PEM) != SSL_SUCCESS)
	{
		DBG_PRINTF(ERROR, "can't load buffer cert file");
		return NULL;
	}
	else if (wolfSSL_CTX_use_PrivateKey_buffer(ssl_ctx, key, key_len, SSL_FILETYPE_PEM) != SSL_SUCCESS)
	{
		DBG_PRINTF(ERROR, "can't load buffer key file");
		return NULL;
	}

	wolfSSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, 0);

    if (0 != ssl_establish((long)tcp_fd, &pssl))
    {
		DBG_PRINTF(ERROR, "ssl establish fail\n");
        return NULL;
    }

    return pssl;
}



int platform_ssl_close(void *ssl)
{
	wolfSSL_shutdown((WOLFSSL*)ssl); //XXX: need?
    wolfSSL_free((WOLFSSL*)ssl);

    if (ssl_ctx)
    {
        wolfSSL_CTX_free(ssl_ctx);
        ssl_ctx = NULL;
    }
    return 0;
}



int platform_ssl_recv(void *ssl, char *buf, int len)
{
    int ret = wolfSSL_read((WOLFSSL*)ssl, buf, len);
    return (ret);
}

int platform_ssl_send(void *ssl, const char *buf, int len)
{
    int ret = wolfSSL_write((WOLFSSL*)ssl, buf, len);
    return (ret);
}
#endif
