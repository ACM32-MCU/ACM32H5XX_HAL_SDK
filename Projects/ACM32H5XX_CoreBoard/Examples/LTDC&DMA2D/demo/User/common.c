 
#include "common.h"  
#include "stdio.h"


/**
 * This function will fill a formatted string to buffer.
 *
 * @param  buf is the buffer to save formatted string.
 *
 * @param  size is the size of buffer.
 *
 * @param  fmt is the format parameters.
 *
 * @return The number of characters actually written to buffer.
 */
int rt_snprintf(char *buf, uint32_t size, const char *fmt, ...)
{
#if 0
    int32_t n;
//    va_list args;
	__va_list args;

    va_start(args, fmt);
    n = rt_vsnprintf(buf, size, fmt, args);
    va_end(args);

    return n;
#endif
	return size;
}



