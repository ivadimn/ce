format ELF64
public _start

section '.data' writeable
    new_line equ 0xA
    msg db "Hello, world!", new_line, 0
    len = $ - msg

section '.bss' writeable
    bss_char rb 1

section '.text' executable
_start:
    mov rax, 2300000        ; помещаем в rax символ
    call print_number
    mov rax, '*'
    call print_char
    mov rax, 725
    call print_number
    mov rax, '='
    call print_char
    mov rax, 10
    mov rbx, 725
    mul rbx
    call print_number   ; вызываем функцию печати числа
    call print_line     ; печатаем переводс строки
    call exit

section '.print_number' executable
print_number:
    push rax
    push rbx
    push rcx
    push rdx

    xor rcx, rcx
    mov rbx, 10
    
    .next_iter:
        xor rdx, rdx        ; в rdx помещается остаток отделения    
        div rbx             ; в div делимое всегда берётся из rax     
        add rdx, '0'
        push rdx
        inc rcx
        cmp rax, 0
        je .print_iter
        jmp .next_iter
    .print_iter:
        cmp rcx, 0
        je .close
        pop rax
        call print_char
        dec rcx
        jmp .print_iter 
    .close:
        pop rdx
        pop rcx
        pop rbx       
        pop rax
        ret

section '.print_char32' executable
; | input
; rax = char
print_char32:
    push rax
    push rbx
    push rcx
    push rdx

    mov [bss_char], al

    mov rax, 4
    mov rbx, 1
    mov rcx, bss_char
    mov rdx, 1
    int 0x80
    pop rdx
    pop rcx
    pop rbx       
    pop rax
    ret


section '.print_char' executable
; | input
; rax = char
print_char:
    
    push rax
    push rbx
    push rcx
    push rdx
    push rax

    mov rsi, rsp       ;  rsp указывает на последнее значение в стеке т.е. на RAX ('W')                       
    mov rax, 1         ;  1 - write вместо 4 в 32 битном режиме 
    mov rdi, 1         ;  stdout = 1 в rdi вместо 0 в rbx в 32 битном режиме  
    mov rdx, 1        ;  в rdx кладётся длина
    syscall
    
    pop rax
    pop rdx
    pop rcx
    pop rbx
    pop rax
    
    ret

section '.print_line' executable
print_line:
    push rax
    mov rax, 0xA
    call print_char
    pop rax
    ret

section '.exit' executable
exit:
    mov rax, 1 ; - указание операционной системе что exit
    mov rbx, 0 ; - возвращаемое программорй значание return (0 - ошибок нет)
    int 0x80