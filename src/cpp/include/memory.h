#ifndef __TEOS_MEMORY_H__
#define __TEOS_MEMORY_H__

extern "C"
{
#include <stdint.h>
}

void memcpy(void *dest, void *src, uint64_t count);
void memset(void *dest, unsigned char sval, uint64_t count);

#endif
