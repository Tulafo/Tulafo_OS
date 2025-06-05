[bits 16]
; AX = NUMBER OF SECTORS TO READ
; BX = MEMORY BUFFER OFFSET WHERE TO LOAD
; SI* = STARTING SECTOR (64 bit)
; ES = MEMORY SEGMENT WHERE TO LOAD
; DL = Drive number

load_sectors:
    mov ax, [HARD_DISK]
    test ax, ax
    jz .use_standard_calls

    ; Set up disk packet
    mov word [sectors_to_load], ax
    mov word [loading_segment], es
    mov word [loading_offset], bx

    mov bx, first_sector
    mov word ax, [si]
    mov word [bx], ax      
    
    add bx, 2
    add si, 2
    mov word ax, [si]
    mov word [bx], ax

    add bx, 2
    add si, 2   
    mov word ax, [si]             
    mov word [bx], ax

    add bx, 2
    add si, 2  
    mov word ax, [si]            
    mov word [bx], ax               


    mov si, disk_packet
    mov ah, 0x42                ; int 0x13 function to read sectors from drive
    int 0x13

    jmp .done_load_kernel


.use_standard_calls:
    mov ah, 0x0e
    mov al, 'H'
    int 0x10
    jmp $



.done_load_kernel:
    ret



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

    



