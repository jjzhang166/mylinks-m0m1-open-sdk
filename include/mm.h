/*=============================================================================+
|                                                                              |
| Copyright 2015                                                            |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*!
*   \file 
*   \brief
*   \author Montage
*/

#ifndef _MM_H_
#define _MM_H_

#include <stddef.h>              // for size_t
#define CONFIG_MM_REGIONS 1
/* #define CONFIG_SMALL_MEMORY */

#ifdef CONFIG_SMALL_MEMORY
# define MM_MIN_SHIFT      4  /* 16 bytes */
# define MM_MAX_SHIFT     15  /* 32 Kb */
#else
# define MM_MIN_SHIFT      4  /* 16 bytes */
# define MM_MAX_SHIFT     18  /* 256 Kb */
#endif

#ifdef CONFIG_SMALL_MEMORY
# define SIZEOF_MM_ALLOCNODE   4
#else
# define SIZEOF_MM_ALLOCNODE   8
#endif

#ifdef CONFIG_SMALL_MEMORY
# define MM_ALLOC_BIT    0x8000
#else
# define MM_ALLOC_BIT    0x80000000
#endif

#ifdef CONFIG_SMALL_MEMORY
# define SIZEOF_MM_FREENODE     8
#else
# define SIZEOF_MM_FREENODE    16
#endif

/* All other definitions derive from these two */

#define MM_MIN_CHUNK     (1 << MM_MIN_SHIFT)
#define MM_MAX_CHUNK     (1 << MM_MAX_SHIFT)
#define MM_NNODES        (MM_MAX_SHIFT - MM_MIN_SHIFT + 1)

#define MM_GRAN_MASK     (MM_MIN_CHUNK-1)
#define MM_ALIGN_UP(a)   (((a) + MM_GRAN_MASK) & ~MM_GRAN_MASK)
#define MM_ALIGN_DOWN(a) ((a) & ~MM_GRAN_MASK)

struct mm_allocnode_s
{
  size_t size;           /* Size of this chunk */
  size_t preceding;      /* Size of the preceding chunk */
};
struct mm_freenode_s
{
  size_t size;                     /* Size of this chunk */
  size_t preceding;                /* Size of the preceding chunk */
  struct mm_freenode_s *flink; /* Supports a doubly linked list */
  struct mm_freenode_s *blink;
};


#endif
