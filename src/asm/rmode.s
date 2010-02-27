global SetupRMode
extern PMode

bits 32

rmodeGDT:
	dw 39 ; 5 entries
	dd rmodeGDT
	dw 0 ; finish null descriptor
	; base = 0x0000
	; limit = 0xFFFF
	; Present, ring 0, executable, readable, byte granularity, 16 bit
	dd 0x0000FFFF
	dd 0x00009A00
	; base = 0x0000
	; limit = 0xFFFF
	; Present, ring 0, data, writable, byte granularity, 16 bit
	dd 0x0000FFFF
	dd 0x00009200
	; base = 0x00000000
	; limit = 0xFFFFFF
	; Present, ring 0, executable, readable, page granularity, 32 bit
	dd 0x0000FFFF
	dd 0x00CF9A00
	; base = 0x00000000
	; limit = 0xFFFFFF
	; Present, ring 0, data, writable, page granularity, 32 bit
	dd 0x0000FFFF
	dd 0x00CF9200

RMIDTptr:
	dw 0x03FF ; 256 4-byte entries
	dd 0 ; IVT located at 0x0000

SetupRMode:

	; We're in protected mode, need to switch back to real...
	cli

	lidt [RMIDTptr]


	lgdt [rmodeGDT] ; load GDT

	jmp 0x08:setCS

bits 16
setCS:

	mov ax, 0x10 ; reset segment registers
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov eax, cr0
	and eax, DWORD 0xFFFFFFFE ; unset pmode bit
	mov cr0, eax

	mov ax, 0x0000 ; reset segment registers
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	xor esp, esp
	xor ebp, ebp
	mov sp, Stack.end

	jmp 0x00:RMode

RMode:
	; TODO: Get information from BIOS

memory:
	mov ax, 0x07E0
	mov es, ax
	mov di, 8 ; leave room below interrupt struct for size

	xor ebx, ebx		; ebx must be 0 to start
	xor bp, bp		; keep an entry count in bp
	mov edx, 0x0534D4150	; Magic value SMAP
	mov eax, 0xe820 ; interrupt function
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry if 20bytes returned
	mov ecx, 24		; ask for 24 bytes - may return 20
	int 0x15
	jc short .failed	; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150	; Some BIOSes apparently trash this register
	cmp eax, edx		; on success, eax must have been reset to "SMAP" magic value
	jne short .failed
	test ebx, ebx		; ebx = 0 implies list is null (worthless)
	je short .failed
	jmp short .jmpin
	.e820lp:
	mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry again
	mov ecx, 24		; ask for 24 bytes again
	int 0x15
	jc short .e820f		; carry set means "end of list already reached", finished
	mov edx, 0x0534D4150	; repair potentially trashed register
	.jmpin:
	jcxz .skipent		; skip any 0 length entries (CX = 0)
	cmp cl, 20		; got a 24 byte ACPI 3.X response?
	jbe short .notext ; if not, next test useless
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je short .skipent ; if so, skip this entry
	.notext:
	mov ecx, [es:di + 8]	; get lower dword of memory region length
	test ecx, ecx		; is the qword == 0?
	jne short .goodent ; if not 0, good entry
	mov ecx, [es:di + 12]	; get upper dword of memory region length
	jecxz .skipent		; if length qword is 0, skip entry
	.goodent:
	inc bp			; got a good entry: ++count, move to next storage spot
	add di, 24
	.skipent:
	test ebx, ebx		; if ebx resets to 0, list is complete
	jne short .e820lp
	.e820f:
	;mov [mmap_ent], bp	; store the entry count
	jmp video			; continue on
	.failed:
	hlt ; TODO: "function unsupported" error, exit

	; TODO: Arrange in proper structure, store pointer in some way
	xor di, di ; write pointer
	mov si, 8 ; read pointer
	xor ecx, ecx
	mov DWORD [es:di], ecx ; first dword is 0
	mov cx, bp ; set loop counter
	mov DWORD [es:di + 4], ecx ; second dword holds count
	add di, 8 ; move to start of BIOS data
arrange:
	mov eax, DWORD [es:si + 16] ; region type
	cmp eax, 1
	je .good
	; bad
	add si, 24
	loop arrange ; decrease ECX (count), jump to arrange
	jmp video ; if ECX = 0, we're done
	.good:
	mov eax, DWORD [es:si]
	mov ebx, eax
	add ebx, DWORD [es:si + 8]
	mov DWORD [es:di], eax ; first dword of start address
	mov DWORD [es:di + 8], ebx ; first word of end address
	mov eax, DWORD [es:si + 4]
	mov ebx, eax
	add ebx, DWORD [es:si + 12]
	mov DWORD [es:di + 4], eax ; second dword of start address
	mov DWORD [es:di + 12], ebx ; second dword of end address
	add si, 24 ; advance to next info struct
	add di, 16 ; advance to next destination
	loop arrange ; decrease count, jump to arrange
	; we're done!
	; TODO: maybe store a pointer at this point or something

video:

finished:
	; Return to protected mode

	mov eax, cr0
	or eax, 0x00000001 ; set PE flag
	mov cr0, eax

	jmp 0x18:setPMode ; reset CS
bits 32
setPMode:
	mov ax, 0x20 ; load segment registers
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	jmp PMode

align 8
Stack:
	resb 4096
	.end:
