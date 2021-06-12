[bits 32]

MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

; section .multiboot
    jmp _start

[extern _edata]
[extern _end]
[extern kmain]
[global _start]

mheader:
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
    dd mheader
    dd _start
    dd _edata
    dd _end
    dd _start
    dd 0
    dd 1024
    dd 768
    dd 32

[BITS 32]
section .text
    align 16
_start:
    mov esp, stack_end
    push esp
    push ebx
    cli
    call kmain
    jmp $


section .bss
    align 16
stack_begin:
        resb 4096
stack_end:
    align 16

