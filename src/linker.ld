OUTPUT_FORMAT(elf64-x86-64)
ENTRY(loader)

KERNEL_LMA = 0x100000;
KERNEL_VMA = 0xFFFF800000000000;
REAL_ADDR = 0x00000500;

SECTIONS
{
    . = REAL_ADDR;

	.rmode : AT(KERNEL_LMA)
	{
		start_rmode = KERNEL_LMA;
		src/asm/rmode.o (.text)
		end_rmode = KERNEL_LMA + SIZEOF(.rmode);
	}

	. = KERNEL_LMA + SIZEOF(.rmode);

    .bootstrap ALIGN(4096) : AT(ADDR(.bootstrap))
    {
        src/asm/loader.o (.text)
    }

    . += KERNEL_VMA;

    .text ALIGN(4096) : AT(ADDR(.text) - KERNEL_VMA)
    {
        _code = .;
        *(EXCLUDE_FILE(*src/asm/loader.o *src/asm/rmode.o) .text)
        *(.rodata*)
        . = ALIGN(4096);
    }

   .data : AT(ADDR(.data) - KERNEL_VMA)
   {
		start_ctors = .;
		*(.ctor*)
		end_ctors = .;
		start_dtors = .;
		*(.dtor*)
		end_dtors = .;
        _data = .;
        *(.data)
        . = ALIGN(4096);
   }

   .ehframe : AT(ADDR(.ehframe) - KERNEL_VMA)
   {
       _ehframe = .;
       *(.ehframe)
        . = ALIGN(4096);
   }

   .bss : AT(ADDR(.bss) - KERNEL_VMA)
   {
       _bss = .;
       *(.bss)

       /*
        * You usually need to include generated COMMON symbols
        * under kernel BSS section or use gcc's -fno-common
        */

        *(COMMON)
       . = ALIGN(4096);
   }

   _end = .;

   /DISCARD/ :
   {
        *(.comment)
   }
}
