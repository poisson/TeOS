extern "C"
{
#include <stdint.h>
}

#include "textmode.h"

const uint64_t KERNEL_VBASE = 0xFFFF800000000000;

extern "C" void kmain(uint64_t* memmap);

void kmain(uint64_t* memmap)
{
    //mbd->boot_loader_name += KERNEL_VBASE;
    uint64_t count;
    count = memmap[0]; // warnings as errors.... T_T
    /* Print a letter to screen to see everything is working: */
    //videoram[0] = ((char*)mbd->boot_loader_name)[0]; /* character 'A' */

    //const char* boot_loader_name = "TeOS\n";

    vterm videoram((unsigned char *)0xFFFF8000000b8000, 0x01);

	for (int i = 0; i < 33; i++)
		videoram << "F";
	//videoram << boot_loader_name;
	//videoram << "A char is assumed to be 8 bits.\n";
	//videoram << "The size of short is " << sizeof(short)*8 << " bits.\n";
	//videoram << "The size of int is " << sizeof(int)*8 << " bits.\n";
	//videoram << "The size of long is " << sizeof(long)*8 << " bits.\n";
	//videoram << "The size of long long is "<< sizeof(long long)*8 << " bits\n";
	
    // Parse memory map to get a stack of usable page frames
}
