[GLOBAL copy_page_physical]
copy_page_physical:
   push ebx
   pushf

   cli

   mov ebx, [esp+12]
   mov ecx, [esp+16]

   mov edx, cr0
   and edx, 0x7fffffff
   mov cr0, edx

   mov edx, 1024

.loop:
   mov eax, [ebx]
   mov [ecx], eax
   add ebx, 4
   add ecx, 4
   dec edx
   jnz .loop

   mov edx, cr0
   or  edx, 0x80000000
   mov cr0, edx

   popf
   pop ebx
   ret

[GLOBAL read_eip]
read_eip:
    pop eax
    jmp eax

[GLOBAL jump_usermode]
extern test_user_function
jump_usermode:
    mov ax, (4 * 8) | 3
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push (4 * 8) | 3
    push eax
    pushf
    push (3 * 8) | 3
    push test_user_function
    iret
