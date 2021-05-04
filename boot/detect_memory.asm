memory_region_count:
    global memory_region_count
    db 0

detect_memory:
    mov ax, 0
    mov es, ax
    mov di, 0x5000
    mov edx, 0x534D4150
    xor ebx, ebx

    .repeat:
        mov eax, 0xE820
        mov ecx, 24
        int 0x15

        cmp ebx, 0
        je .finished

        add di, 24
        inc byte [memory_region_count]
        jmp .repeat

    .finished:
    ret
