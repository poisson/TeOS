global higherhalf, KERNEL_VBASE
extern kmain, start_ctors, end_ctors, start_dtors, end_dtors, PML4

bits 64

KERNEL_VBASE equ 0xFFFF800000000000 ; kernel virtual address base
STACKSIZE equ 0x4000 ; 16 kibibytes of temporary kernel stack space

section .text
higherhalf:
	mov QWORD [rcx], 0 ; RCX has PML4 address
	invlpg [0] ; These two lines undo the ID mapping of the first 4 mebibytes

StartHH:

	mov rsp, stack+STACKSIZE ; sets up stack

static_ctors_loop:
	mov rbx, start_ctors
	jmp .test
	.body:
	call [rbx]
	add rbx, 8
	.test:
	mov rcx, end_ctors
	cmp rbx, rcx
	jb .body

	; Parameter passing goes here
	mov rdi, 0xFFFF800000007E00 ; vmem pointer to memory structure
	; TODO: restructure out the magic numbers

	call kmain

	static_dtors_loop:
	mov rbx, start_dtors
	jmp .test
	.body:
	call [rbx]
	add rbx, 8
	.test:
	mov rcx, end_dtors
	cmp rbx, rcx
	jb .body

hang:
	hlt
jmp hang

section .bss
stack:
	resb STACKSIZE
