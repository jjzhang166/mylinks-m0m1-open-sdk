/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file 
*   \brief CAMELOT 
*   \author Montage
*/

#ifndef _FLASH_API_H
#define _FLASH_API_H

enum
{
    FL_OK           = 0,
    FL_ERR          = -1,
    FL_ERR_PROG     = -2,
    FL_ERR_ERASE    = -3,
    FL_ERR_TIMEOUT  = -4,
    FL_ERR_UNSUPPORT= -5,
    FL_ERR_NOT_INIT = -6,
    FL_BOTTOM       = 1,
    FL_TOP          = 2,
    FL_PROG_UNIT = (1<<16),
};

typedef struct flashsec
{
    unsigned char size; // in power of 2
    unsigned short num; // number of sector
    unsigned char ecmd;
} flashsec;

struct flash_dev_tab
{
    unsigned long id;
    char flags;
    short sec;
    flashsec * sdesc;
    unsigned long size;
};
#define FLASH_APP_STARTADDR  0x20000
#define FLASH_OTA_STARTADDR  0x80000
#define FLASH_OTA_LENGTH     0x60000
#define USER_PARAM_FLASH_PART       0xE0000
#define USER_PARAM_FLASH_LENGTH       0x10000 // 64k bytes
#define WEB_PARAM_FLASH_PARAT   USER_PARAM_FLASH_PART
#define WEB_PARAM_FLASH_LENGTH  USER_PARAM_FLASH_LENGTH


int flash_erase (unsigned int addr, unsigned int len);
int flash_write (unsigned int addr, unsigned int from, int len);
unsigned int flash_read(unsigned int addr, unsigned int to, unsigned int numOfByte);

#endif
