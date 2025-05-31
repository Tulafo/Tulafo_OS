[bits 16]

KERNEL_SECTORS equ 32
KERNEL_FIRST_SECTOR equ 3


load_kernel:
    mov si, msg_loading
    call print_string

    mov ax, [HARD_DISK]
    test ax, ax
    jz .use_standard_calls

    ; Set up disk packet
    mov word [sectors_to_load], KERNEL_SECTORS
    mov word [loading_offset], KERNEL_LOCATION
    mov word [first_sector], KERNEL_FIRST_SECTOR    ; Load from fourth sector
    mov word [first_sector + 2], 0                  ; Clear upper 6 bytes
    mov word [first_sector + 4], 0                  ; Clear upper 6 bytes
    mov word [first_sector + 6], 0                  ; Clear upper 6 bytes


    mov si, disk_packet
    mov ah, 0x42                ; int 0x13 function to read sectors from drive
    mov dl, [BOOT_DISK]         ; drive number
    int 0x13

    jc error_loading_disk
    jmp .done_load_kernel


.use_standard_calls:
    mov ah, 0x02                    ; int 0x13 function to read sectors from disk
    mov al, KERNEL_SECTORS          ; How many sectors to read (1-128), try increase if something breaks
    mov ch, 0                       ; Cylinder number
    mov cl, KERNEL_FIRST_SECTOR+1   ; Sector number
    mov dh, 0                       ; Head number
    mov dl, [BOOT_DISK]             ; Disk number
    mov bx, KERNEL_LOCATION         ; Where to load kernel (duh)
    int 0x13

    jc error_loading_disk


.done_load_kernel:
    call new_line
    mov si, msg_loaded
    call print_string
    ret



error_loading_disk:
    mov si, msg_err_lod_kernel
    call print_string
    call new_line

    mov si, msg_err_code
    call print_string

    mov al, ah
    mov ah, 0
    mov di, err_code
    call num_to_string_hex
    mov si, err_code
    call print_string

    jmp $



