format ELF64

public exit

section '.exit' executable
exit:
    mov rax, 0x3c   ; - указание операционной системе что exit
    mov rdi, 0      ; - возвращаемое программорй значание return (0 - ошибок нет)
    syscall