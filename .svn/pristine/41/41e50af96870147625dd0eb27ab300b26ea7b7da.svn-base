/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file lynx_debug.h
*   \brief  debug function.
*   \author Montage
*/

#ifndef _LYNX_DEBUG_H_
#define _LYNX_DEBUG_H_
#include <mt_types.h>
#include <lynx_dev.h>

enum {
	INFO_NO_PREFIX = 0,
	INFO,
	WARN,
	ERROR,
	PANIC,
};

void dbg_printf(char dbg, char *fmt, ...);

#define DBG_PRINTF(_lv, ...) \
	do {                           \
		if ((_lv) >= ldev->debug_level) \
			dbg_printf(_lv, __VA_ARGS__);     \
	} while(0)

#endif //_LYNX_DEBUG_H_
