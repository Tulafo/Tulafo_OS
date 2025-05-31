GDT_Start:
    null_descriptor:
        dd 0    
        dd 0

    code_descriptor:
        dw 0xffff       ;limit
        dw 0            ;16 bits +
        db 0            ;8 bits = 24 -> First 24 bits of the base (32 bit)
        db 10011010b    ;Pres, priv, tyope + Type flags

        db 11001111b    ;Other flags + limit
        db 00000000b    ;Last 8 bits of the base (32 bit)

    data_destriptor:
        ;Same as above but flags type flags changed: 1010 -> 0010
        dw 0xffff
        dw 0
        db 0
        db 10010010b
        db 11001111b
        db 00000000b

    video_descriptor:
    dw 0xFFFF             ; Limit (0xFFFF)
    dw 0x8000             ; Base (0xB8000, parti bassa e media)
    db 0x0B               ; Base (parte alta)
    db 10010010b          ; Flags: Present, DPL=00, S=1, Type=0010 (Data, Read/Write)
    db 11001111b          ; Flags: G=1, D/B=1, Limit (alta)=1111
    db 0x00               ; Base (ultimi 8 bit)


GDT_End:


GDT_Descriptor:
    dw GDT_End - GDT_Start - 1      ;Size
    dd GDT_Start                    ;Start

    CODE_SEG equ code_descriptor - GDT_Start
    DATA_SEG equ data_destriptor - GDT_Start
    VIDEO_SEG equ video_descriptor - GDT_Start