#ifndef __MFS_H
#define __MFS_H

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,			/* 0: Successful */
	RES_ERROR = -1,		/* 1: R/W Error */
	RES_WRPRT = -2,		/* 2: Write Protected */
	RES_NOTRDY = -3,	/* 3: Not Ready */
	RES_PARERR = -4		/* 4: Invalid Parameter */
} DRESULT;

typedef enum
{
	SEEK_SET = 0,//参数offset即为新的读写位置
	SEEK_CUR,//从当前的读写位置往后增加offset个偏移地址
	SEEK_END,//将读写位置指向文件尾后增加0ffset个偏移地址
} SEEK;

struct minifs{
	//文件系统地址
	uint32_t start;
	//文件长度
	uint32_t fillen;
	//文件偏移地址
	uint32_t offset;
};

struct minfo{
	uint32_t start;
	uint32_t fils;
	uint32_t version;
	uint32_t ctime;
};

//uint32_t addr:文件系统.img文件在flash开始的位置
extern DRESULT mfmount(uint32_t addr);

//uint32_t fname:需要打开的文件
extern DRESULT mfopen(struct minifs *fs,char *fname);

//uint8_t buf:读取的缓冲开始地址,uin32_t len，读取长度
//返回:读取的文件长度
extern uint32_t mfread(struct minifs *fs,uint8_t *buf,uint32_t len);

//返回:获取文件长度
extern uint32_t mftell(struct minifs *fs);

//uint32_t offset:位置地址,int fromwhere:从何处移动相对地址
//返回:当前文件的读取位置
extern uint32_t mfseek(struct minifs *fs, long offset,SEEK fromwhere);

#endif

