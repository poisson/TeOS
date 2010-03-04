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

	videoram << 256;

    // Parse memory map to get a stack of usable page frames
}
