#ifndef	WWW_DESC_H
#define	WWW_DESC_H

#define	PAGE_COMP	(1<<0)
#define	PAGE_NO_CACHE	(1<<1)
#define	PAGE_NO_AUTH	(1<<2)
#define PAGE_CGI	(1<<3)
#define PAGE_ADMIN	(1<<4)
#define PAGE_WEBAUTH (1<<5)
#define PAGE_PSEUDO	(1<<6)

#define	WWW_MAGIC	0xAACC8000

#define	_TZ_OFS		28800
#define PAGE_ENTRY 	0 //dummy
#define ALIGN1 __attribute__ ((section (".www_data"), aligned (0x2)))

#define DEC_WWW(_desc,_time,_page_num,_page_data_sz,_page_data_ofs) \
	static struct www_desc _web ALIGN1 = {magic: WWW_MAGIC, desc: _desc, \
		time: _time+_TZ_OFS, len: 0xfe&(sizeof(_desc)+1)+sizeof(struct www_desc), \
		page_num: _page_num, page_data_sz: _page_data_sz, \
		page_data_ofs: _page_data_ofs}
#define DEC_PAGE(_idx,_name,_size,_flag,_len) \
	static struct page_desc _p ## _idx ALIGN1 = {name: _name, size: _size, \
		flag: _flag, len: 0xfe&(sizeof(_name)+1)+sizeof(struct page_desc)}

struct page_desc
{
	unsigned char flag;
	unsigned char len;		//this header length
	unsigned int size;		//page data size 
	char name[];			//variable length filename
} __attribute__ ((packed));

struct www_desc
{
	unsigned long magic;	// header
	unsigned long time;		// create time
	short len;				// this desc_size
	short page_num;			// number of page
	int page_data_sz;		// page data size
	short page_data_ofs;	// page data offset
	char desc[];
} __attribute__ ((packed));

#endif //WWW_DESC_H
