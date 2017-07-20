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


#ifndef _AES_WRAP_H_
#define _AES_WRAP_H_


#include <mt_types.h>

int aes_wrap(const u8 *kek, int n, const u8 *plain, u8 *cipher);
int aes_unwrap(const u8 *kek, int n, const u8 *cipher, u8 *plain);

#endif  // _AES_WRAP_H_
