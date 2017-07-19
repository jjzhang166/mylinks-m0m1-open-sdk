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

#ifndef IMAGE_H
#define IMAGE_H

struct img_head
{
    unsigned char magic[3];
    unsigned char hlen; //header length
	unsigned int size;  // image size
    unsigned int run;   // where to run

	unsigned short rver;   // ROM ver
    unsigned short aver;   // App ver
	unsigned short flags;   // flags
    unsigned short mid; // model

    unsigned int next;  // chain to next?
	unsigned int time;  // time when building image
 
    unsigned short chksum;  // 
    unsigned short resv;    // 

}__attribute__((packed)) ; 

#define IH_MAGIC    { 'M', 'T', 'X' }
#define IH_LEN		sizeof(struct img_head)
#define IH_CHKSUM_EN    (1<<2)
#define IH_NEXT_IMG     (1<<3)

#if defined(CONFIG_ROM_VER)
#define ROM_VER		CONFIG_ROM_VER
#else
#define ROM_VER		1
#endif

enum {
	IH_MODEL_SFLASH = 1,
	IH_MODEL_UART,
	IH_MODEL_SDIO,
	IH_MODEL_USB,
};
			
#endif /* IMAGE_H */
