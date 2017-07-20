#ifndef __BYTEORDER_H_
#define __BYTEORDER_H_
#include <mt_types.h>
#include <arch/cpu.h>

/*
 * casts are necessary for constants, because we never know how for sure
 */
#define constant_swab16(x) ((__u16)(				\
	(((__u16)(x) & (__u16)0x00ffU) << 8) |			\
	(((__u16)(x) & (__u16)0xff00U) >> 8)))

#define constant_swab32(x) ((__u32)(				\
	(((__u32)(x) & (__u32)0x000000ffUL) << 24) |		\
	(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) |		\
	(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) |		\
	(((__u32)(x) & (__u32)0xff000000UL) >> 24)))

#define constant_swab64(x) ((__u64)(				\
	(((__u64)(x) & (__u64)0x00000000000000ffULL) << 56) |	\
	(((__u64)(x) & (__u64)0x000000000000ff00ULL) << 40) |	\
	(((__u64)(x) & (__u64)0x0000000000ff0000ULL) << 24) |	\
	(((__u64)(x) & (__u64)0x00000000ff000000ULL) <<  8) |	\
	(((__u64)(x) & (__u64)0x000000ff00000000ULL) >>  8) |	\
	(((__u64)(x) & (__u64)0x0000ff0000000000ULL) >> 24) |	\
	(((__u64)(x) & (__u64)0x00ff000000000000ULL) >> 40) |	\
	(((__u64)(x) & (__u64)0xff00000000000000ULL) >> 56)))

#define constant_swahw32(x) ((__u32)(			\
	(((__u32)(x) & (__u32)0x0000ffffUL) << 16) |		\
	(((__u32)(x) & (__u32)0xffff0000UL) >> 16)))

#define constant_swahb32(x) ((__u32)(			\
	(((__u32)(x) & (__u32)0x00ff00ffUL) << 8) |		\
	(((__u32)(x) & (__u32)0xff00ff00UL) >> 8)))


#ifdef ___swab16
#define __swab16(x) \
	(__builtin_constant_p((__u16)(x)) ? \
	constant_swab16(x) : \
	___swab16(x))
#else
#define __swab16(x) \
	(constant_swab16((__u16)(x))) 
#endif

#ifdef ___swab32
#define __swab32(x) \
	(__builtin_constant_p((__u32)(x)) ? \
	constant_swab32(x) : \
	___swab32(x))
#else
#define __swab32(x) \
	(constant_swab32((__u32)(x))) 
#endif


extern inline void ___swab16s(volatile unsigned short *addr)
{
#if 1 //avoid unailgned access
     unsigned char c=*(unsigned char*)addr;
     *((unsigned char *)addr) = *(((unsigned char *)addr) +1);
     *(((unsigned char *)addr) +1) = c;
#else
     unsigned short result = *addr;
     *addr =  ___swab16(result);
#endif
}

#define cpu_to_le16 __swab16
#define cpu_to_le32 __swab32
#define le16_to_cpu __swab16
#define le32_to_cpu __swab32
//// FIXME
#define le16_to_cpus  __swab16s

#define cpu_to_be32(a) (a)

#endif

