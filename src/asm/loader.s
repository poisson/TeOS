global loader, PML4, PMode
extern higherhalf, start_rmode, end_rmode, SetupRMode, RMode

bits 32

MODULEALIGN equ 1<<0 ; still don't get this
MEMINFO equ 1<<1 ; "provide memory map"?
FLAGS equ MODULEALIGN | MEMINFO ; flags for the multiboot header
MAGIC equ 0x1BADB002 ; magic number for multiboot header
CHECKSUM equ -(MAGIC + FLAGS) ; checksum for multiboot header

REAL_ADDR equ 0x00000500 ; location of real mode code below 1MB
KERNEL_VBASE equ 0xFFFF800000000000 ; kernel virtual address base
KERNEL_PML4_NUM equ 256 ; 0-based index of the kernel's vbase in the pml4
PML4_ENTRIES equ 512

section .text
align 0x1000 ; 4KB align for pml4
PML4:
	dq PDPT + 0x7 ; first entry points to PDPT - flags user, read/write, present
	times 255 dq 0 ; zero most entries
	dq PDPT + 0x7 ; so does entry 512 - first of upper half
	times 255 dq 0 ; zero rest of entries
PDPT:
	dq PDT + 0x7 ; points to PDT - flags user, read/write, present
	times 511 dq 0 ; zero the rest
PDT:
	dq 0x87 ; present, read/write, 2MiB - 1st 2 mebibytes
	dq 0x87 ; present, read/write, 2MiB - 2nd 2 mebibytes
	times 510 dq 0 ; zero the rest


mbi dd 0 ; will store pointer temporarily to multibootinfo structure
magic dd 0 ; will store magic number temporarily

; The upshot of this is that the first 4 mebibytes of both the lower and upper halves
; of the virtual address space point to the first 4 mebibytes of the physical space

align 4 ; required for multiboot

multiboot:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM ; Multiboot header - rest of info is in elf headers

loader:
	; entry point

	mov ecx, end_rmode
	sub ecx, start_rmode ; ECX contains number of bytes for copy
	mov esi, start_rmode
	mov edi, REAL_ADDR
	rep movsb ; move the stuff to the open area below 1MB

	; Check if we are in protected mode
	mov eax, cr0
	test eax, 0x00000001 ; bit 0 set in PM
	jnz SetupRMode

	; TODO: think about what to do if already in real mode. Maybe play with *s.


PMode:
	; Return to protected mode before this happens

	; not relying on GRUB, no longer need to save magic/struct pointer

	cli ; do not want to get interrupt while in inconsistent state

	lgdt [GDTpointer] ; temporary GDT

	; Set up paging
	mov ecx, cr4
	or ecx, 0x20 ; set PAE bit, PSE bit for 2MiB pages
	mov cr4, ecx

	mov ecx, PML4
	mov cr3, ecx ; cr3 holds the address of the PML4

	mov ecx, 0xC0000080 ; MSR index
	rdmsr ; reads MSR into EDX and EAX
	or eax, 0x100 ; enables long mode
	wrmsr

	mov ecx, cr0
	or ecx, 0x80000000 ; enable paging
	mov cr0, ecx

	jmp 0x08:longmode

bits 64
longmode:

	mov cx, 0x10
	mov ds, cx
	mov es, cx
	mov fs, cx
	mov gs, cx
	mov ss, cx

	mov eax, [magic]

	mov rdx, higherhalf
	mov rcx, PML4
	jmp rdx

	hlt

align 8
GDT:
	; null descriptor
	dq 0
	; kernel code descriptor
	.code:
	dd 0 ; base and limit are ignored
	dd 0x00af9a00
	.data:
	dd 0
	dd 0x00209200 ; data segment

GDTpointer:
	dw 23 ; 3 segments at 8 bytes each, minus 1 = size
	dq GDT ; address of GDT itself
