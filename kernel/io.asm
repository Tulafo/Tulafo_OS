[bits 32]

section .text
global add

add:
    mov ebp, esp

    mov eax, [esp+4]  ; Load first argument into EAX
    add eax, [esp+8]  ; Add second argument
    ret               ; Return (EAX holds the result)