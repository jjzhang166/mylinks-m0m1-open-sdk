
/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file sha1.h
*   \brief  sha1 crypto definition
*   \author Montage
*/


#ifndef _SHA_1_H_
#define _SHA_1_H_

#include <mt_types.h>
#include <sys/types.h>
void hmac_sha1(const u8 *data, u32 data_len, const u8 *key, size_t key_len, u8 *mac);
void hmac_sha1_vector(const u8 *key, u32 key_len, u32 num_elem, const u8 *addr[], const u32 *len, u8 *mac);

#endif // _SHA_1_H_
