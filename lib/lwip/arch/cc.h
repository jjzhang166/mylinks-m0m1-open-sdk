/* 
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science. 
 * All rights reserved.  
 *  
 * Redistribution and use in source and binary forms, with or without modification,  
 * are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution. 
 * 3. The name of the author may not be used to endorse or promote products 
 *    derived from this software without specific prior written permission.  
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED  
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF  
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT  
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS  
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN  
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING  
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY  
 * OF SUCH DAMAGE. 
 * 
 * This file is part of the lwIP TCP/IP stack. 
 *  
 * Author: Adam Dunkels <adam@sics.se> 
 * 
 */ 
#ifndef __CC_H__ 
#define __CC_H__ 

#include <mt_types.h>

typedef u8           u8_t; 
typedef s8           s8_t; 
typedef u16          u16_t; 
typedef s16          s16_t; 
typedef u32          u32_t; 
typedef s32          s32_t; 
typedef u32          mem_ptr_t; 

/* Define (sn)printf formatters for these lwIP types */
#define U16_F "d"
#define S16_F "hd"
#define X16_F "hx"
#define U32_F "lu"
#define S32_F "ld"
#define X32_F "lx"
#define SZT_F "uz"

#define BYTE_ORDER BIG_ENDIAN

/* Use LWIP error codes */
#define LWIP_PROVIDE_ERRNO

/* GCC tools */
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(fld) fld
#define ALIGNED(n)  __attribute__((aligned (n)))

/* Used with IP headers only */
#define LWIP_CHKSUM_ALGORITHM   1

/* Plaform specific diagnostic output */
#ifdef CONFIG_WLA_DEBUG_MSG
#define LWIP_PLATFORM_DIAG(msg) { serial_printf msg; }
#define LWIP_PLATFORM_ASSERT(msg) { serial_printf(msg); }
#else
#define LWIP_PLATFORM_DIAG(msg)
#define LWIP_PLATFORM_ASSERT(msg)
#endif

#define LWIP_PLATFORM_HTONS(x)      (x)
#define LWIP_PLATFORM_HTONL(x)      (x)

#endif /* __CC_H__ */ 
