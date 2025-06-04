[bits 16]
; AX = NUMBER OF SECTORS TO READ
; BX = MEMORY BUFFER WHERE TO LOAD
; SI* = STARTING SECTOR (64 bit)

load_kernel:
    mov ax, [HARD_DISK]
    test ax, ax
    jz .use_standard_calls

    ; Set up disk packet
    mov word [sectors_to_load], KERNEL_SECTORS
    mov word [loading_offset], KERNEL_LOCATION
    mov word [first_sector], 3                      ; Start of boot partition
    mov word [first_sector + 2], 0                  ; Clear upper 6 bytes
    mov word [first_sector + 4], 0                  ; Clear upper 6 bytes
    mov word [first_sector + 6], 0                  ; Clear upper 6 bytes


    mov si, disk_packet
    mov ah, 0x42                ; int 0x13 function to read sectors from drive
    mov dl, [BOOT_DISK]         ; drive number
    int 0x13

    jmp .done_load_kernel


.use_standard_calls:
    mov ah, 0x0e
    mov al, 'H'
    int 0x10
    jmp $



.done_load_kernel:
    ret


    



