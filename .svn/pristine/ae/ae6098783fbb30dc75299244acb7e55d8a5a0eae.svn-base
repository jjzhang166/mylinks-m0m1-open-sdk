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
/*=============================================================================+
| Included Files                                                               |
+=============================================================================*/
#include <flash_api.h>
/*=============================================================================+
| Serial Flash Database                                                        |
+=============================================================================*/
#if CONFIG_SFLASH
#define SF_ID_MD25D40       0x51401300  /* 4Mb */
#define SF_ID_MD25D10       0xc8401100  /* 1Mb */
#define SF_ID_W25X40        0xef301300  /* 4Mb */

#if 0/* supported uniform 64KB sflash */
#define SF_ID_EON25F40      0x1c31131c  /* 4Mb */
#define SF_ID_EON25P80      0x1c20141c  /* 8Mb */
#define SF_ID_EON25F80      0x1c31141c  /* 8Mb */
#define SF_ID_EON25P16      0x1c20151c  /* 16Mb */
#define SF_ID_EON25F16      0x1c31151c  /* 16Mb */
#define SF_ID_EON25Q32A     0x1c30161c  /* 32Mb */
#define SF_ID_EON25P32      0x1c20161c  /* 32Mb */
#define SF_ID_MX25L4005     0xc22013c2  /* 4Mb */
#define SF_ID_MX25L8006E    0xc22014c2  /* 8Mb */
#define SF_ID_MX25L1605     0xc22015c2  /* 16Mb */
#define SF_ID_MX25L6445E    0xc22017c2  /* 64Mb */
#define SF_ID_MX25L12845E   0xc22018c2  /* 128Mb */
#define SF_ID_S25FL128P     0x01201803  /* 128Mb */
#define SF_ID_W25X10        0xef301100  /* 1Mb */
#define SF_ID_W25X20        0xef301200  /* 2Mb */
#define SF_ID_W25X40        0xef301300  /* 4Mb */
#define SF_ID_W25X80        0xef301400  /* 8Mb */
#define SF_ID_W25X16        0xef301500  /* 16Mb */
#define SF_ID_W25X32        0xef301600  /* 32Mb */
#define SF_ID_W25X64        0xef301700  /* 64Mb */
#define SF_ID_W25Q80BV      0xef401400  /* 8Mb */
#define SF_ID_W25Q16BV      0xef401500  /* 16Mb */
#define SF_ID_W25Q64BV      0xef401700  /* 64Mb */
#define SF_ID_W25Q128BV     0xef401800  /* 128Mb */
#endif

enum
{
// stype 1
    EON         = 0x1c,
    MACRONIX    = 0xc2,
    SPANSION    = 0x01,
    ST          = 0x20,
    WINBOND     = 0xef,
    GigaDevice  = 0x51,
	GigaDevice1 = 0xc8,
// stype 2
    ATMEL       = 0x1f,
// stype 3
    SST         = 0xbf,
};

/* {sector size, sector num} */
const flashsec uniform_64KB[9] = {
{16, 1},                    //64KB*1;   64KB
{16, 2},                    //64KB*2;   128KB
{16, 4},                    //64KB*4;   256KB
{16, 8},                    //64KB*8;   512KB
{16, 16},                   //64KB*16;  1MB
{16, 32},                   //64KB*32;  2MB
{16, 64},                   //64KB*64;  4MB
{16, 128},                  //64KB*128; 8MB
{16, 256}                   //64KB*256; 16MB
};

#if 0
#define SF_CMD_SSEC_ERASE   0x20    /** erase 4K sector */
flashsec var_4K_64k_512K[] = {
{16, 1},                            //64kB;
{12, 16, ecmd:SF_CMD_SSEC_ERASE},   //4K*16; 64KB
{16, 6},                            //64k*7; 448KB
};
#endif

#if (CONFIG_ROM_VER > 1)
const struct flash_dev_tab sflash_db[] = {
//  {id, flags, num of sec, sdesc, size }
    {0x0,            (0|0),  0x08, (flashsec *)&uniform_64KB[3] , 0x80000 },
    {SF_ID_MD25D40,  (0|0),  0x08, (flashsec *)&uniform_64KB[3] , 0x80000 },
	{SF_ID_MD25D10,  (0|0),  0x08, (flashsec *)&uniform_64KB[1] , 0x20000 },
    {SF_ID_W25X40,   (0|0),  0x08, (flashsec *)&uniform_64KB[3] , 0x80000 },
};

#else
struct flash_dev_tab sflash_db[] = {
//  {id, flags, num of sec, sdesc, size }
    {0x0,            (0|0),  0x0, 0x0 , 0x000000 },//reserved for uniform 64KB setting
    {0x0,            (0|0),  0x08, (flashsec *)&uniform_64KB[3] , 0x80000 },
    {SF_ID_MD25D40,  (0|0),  0x08, (flashsec *)&uniform_64KB[3] , 0x80000 },
	{SF_ID_MD25D10,  (0|0),  0x08, (flashsec *)&uniform_64KB[1] , 0x20000 },
    {SF_ID_W25X40,   (0|0),  0x08, (flashsec *)&uniform_64KB[3] , 0x80000 },
};
#endif

#define SFL_DEVTAB_SZ (sizeof(sflash_db)/sizeof(struct flash_dev_tab))
#define SFL_DEVTAB_DEFAULT 0

#endif

