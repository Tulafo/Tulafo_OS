[org 0x7e00]
[bits 16]

KERNEL_LOCATION equ 0x1000  ; Where to load in memory
KERNEL_SECTORS equ 63

_stage_2_start:
    mov [HARD_DISK], dh
    mov [BOOT_DISK], dl

    mov si, msg_loading
    call print_string

    call load_kernel
    jc error_loading_kernel

    cli                     ;Disables interrupts
    lgdt [GDT_Descriptor]   ;Loads Global Descriptor Table

    mov eax, cr0
    or eax, 00000001b 
    mov cr0, eax        ;yay, 32 bit mode protected mode!!

    jmp CODE_SEG:start_protected_mode


error_loading_kernel:
    call new_line
    mov si, msg_err_lod_kernel
    call print_string

    mov si, msg_err_code
    call print_string

    mov al, ah
    mov ah, 0
    mov di, err_code
    call num_to_string_hex
    mov si, err_code
    call print_string

    jmp $


[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp

    jmp KERNEL_LOCATION


end:
    jmp $


%include "print_string.asm"
%include "invert_string.asm"
%include "new_line.asm"
%include "load_sectors.asm"
%include "num_to_string.asm"
%include "num_to_string_hex.asm"
%include "GDT.asm"

disk_packet:
    db 0x10             ; Size of packet
    db 0                ; Reserved
sectors_to_load:
    dw 0
loading_offset:
    dw 0                ; Memory ofset
loading_segment:
    dw 0                ; Memory segment
first_sector:
    dq 0                ; Sectors numbers starts from 0: first sector = sector 0

HARD_DISK:
    db 0

BOOT_DISK:
    db 0

msg_loading:
    db 'Loading kernel...',0

msg_sector_needed:
    db 'Sector needed: ',0

msg_sector_read:
    db 'Sector read: ',0

msg_err_lod_kernel:
    db 'Error loading kernel    ',0

msg_err_code:
    db 'Code ',0

err_code:
    times 6 db 0

times 1024 - ($-$$) db 0