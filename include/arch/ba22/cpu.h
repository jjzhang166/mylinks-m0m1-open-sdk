/*=============================================================================+
|                                                                              |
| Copyright 2014                                                              |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file 
*   \brief 
*   \author Montage
*/

#ifndef _CPU_H_
#define _CPU_H_
 
#include <arch/ba22/excutil.h>
#include <arch/ba22/spr_defs.h>

#define MFSPR(reg)   \
({ int __ret;               \
        __asm__ __volatile__(    \
    "b.mfspr\t%0,%1,0\n\t" \
        : "=r" (__ret)  \
		: "r" (reg));    \
        __ret;              \
})

#define MTSPR(reg, val)      \
    __asm__ __volatile__(   \
    "b.mtspr\t%0,%1,0;\n\t" \
    : :  "r" (reg), "r" (val) \
	:"cc");

struct saved_frame {
	unsigned long gprs[31];
	unsigned long epcr;
	unsigned long esr;
	unsigned long eear;
};

#define ffs(x) \
({ int __ret;               \
        __asm__ __volatile__(    \
    "b.ff1\t%0,%1\n\t" \
        : "=r" (__ret)  \
		: "r" (x));    \
        __ret;              \
})

static inline int fls(int x)
{
	int c;
	__asm__ __volatile__("b.clz %0,%1" : "=r" (c) : "r" (x));
	return 32 - c;
}

#define ___swab32(x) \
({ int __ret;               \
        __asm__ __volatile__(    \
    "b.swab\t%0,%1\n\t" \
        : "=r" (__ret)  \
		: "r" (x));    \
        __ret;              \
})

#define ___swab16(x) \
({ int __ret;               \
        __asm__ __volatile__(    \
    "b.swab\t%0,%1\n\t" \
    "b.rori\t%0,%0,16\n\t" \
        : "=r" (__ret)  \
		: "r" (x));    \
        __ret;              \
})

#define barrier() __asm__ __volatile__("": : :"memory")
#define mb() barrier()

#define cpu_trap() \
		(__asm__ __volatile__("b.trap"))

#define DCACHE_LINE_SIZE		0
#define DCACHE_FLUSH(a, l)
#define DCACHE_INVALIDATE(a, l)

#define UNCACHE_BASE		(0x00000000)
#define CACHE_BASE			(0x00000000)
#define	PHYSICAL_ADDR(va) 	((unsigned int)va)
#define	VIRTUAL_ADDR(pa)	((unsigned int)pa)
#define	CACHED_ADDR(va)		((unsigned int)va)
#define	NONCACHED_ADDR(va)	((unsigned int)va)

#define CLK_MEASURE_START	\
	do { \
		MTSPR(SPR_TTCR, 0); \
		MTSPR(SPR_TTMR, SPR_TTMR_SR|SPR_TTMR_PERIOD); \
		MTSPR(SPR_SR, (MFSPR(SPR_SR) | SPR_SR_TEE)); \
	}while(0);

#define CLK_MEASURE_END(result)  \
	do { \
		MTSPR(SPR_SR, (MFSPR(SPR_SR) & ~SPR_SR_TEE)); \
		result = MFSPR(SPR_TTCR); \
	}while(0);

#endif
