format ELF64
public _start

section '.text' executable
_start:
    add rax, 'A'        ; помещаем в rax символ
    call print_string   ; вызываем функцию печати строки

    call exit

section '.print_char' executable
; | input
; rax = char
print_string:
    push rax
    mov rax, 1         ; 
    mov rdi, 1         ; 
    mov rsi, rsp       ; 
    mov rdx, 1
    syscall
    pop rax
    ret

section '.exit' executable
exit:
    mov rax, 1 ; - указание операционной системе что exit
    mov rbx, 0 ; - возвращаемое программорй значание return (0 - ошибок нет)
    int 0x80