#include "memory.h"

void memcpy(void *destination, void *source, uint64_t count)
{
	if (destination == source) return;
	uint64_t* dest = (uint64_t*)destination;
	uint64_t* src = (uint64_t*)source;
	while(count >= 8)
	{
		*dest = *src;
		(dest)++;
		(src)++;
		count -= 8;
	}
	uint32_t* dest32 = (uint32_t*)dest;
	uint32_t* src32 = (uint32_t*)src;
	if(count >= 4)
	{
		*dest32 = *src32;
		(dest32)++;
		(src32)++;
		count -= 4;
	}
	uint16_t* dest16 = (uint16_t*)dest32;
	uint16_t* src16 = (uint16_t*)src32;
	if(count >= 2)
	{
		*dest16 = *src16;
		(dest16)++;
		(src16)++;
		count -= 2;
	}
	if (count >= 1)
	{
		*(uint8_t*)dest16 = *(uint8_t*)src16;
	}
  return;
}

void memset(void *destination, uint8_t sval, uint64_t count) // doesn't work
{
//	uint64_t val = (sval & 0xFF); // create a 64-bit version of 'sval'
//	val |= ((val << 8) & 0xFF00);
//	val |= ((val << 16) & 0xFFFF0000);
//	val |= ((val << 32) & 0xFFFFFFFF00000000);

	uint8_t* dest = (uint8_t*)destination;

	while (count > 0)
	{
		*dest = sval;
		dest++;
		count--;
	}

//	asm ("xchg %bx, %bx");
//
//	uint64_t* dest = (uint64_t*)destination;
//
//	while(count >= 8)
//	{
//		*dest = (uint64_t)val;
//		dest++;
//		count -= 8;
//	}
//
//	asm ("xchg %bx, %bx");
//
//	uint32_t* dest32 = (uint32_t*)dest;
//
//	if(count >= 4)
//	{
//		*dest32 = (uint32_t)val;
//		dest32++;
//		count -= 4;
//	}
//
//	asm ("xchg %bx, %bx");
//
//	uint16_t* dest16 = (uint16_t*)dest32;
//
//	if(count >= 2)
//	{
//		*(uint16_t*)dest = (uint16_t)val;
//		dest16++;
//		count -= 2;
//	}
//
//	asm ("xchg %bx, %bx");
//
//	uint8_t* dest8 = (uint8_t*)dest16;
//
//	if(count >= 1)
//	{
//		*dest8 = (uint8_t)val;
//	}
//
//	asm ("xchg %bx, %bx");

	return;
} //page fault for some reason

