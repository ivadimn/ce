format ELF64
public _start

section '.data' writeable
    _buffer.size equ 32

section '.bss' executable
    _buffer rb _buffer.size

section '.text' executable
_start:
   
    mov rax, 571
    mov rbx, _buffer
    mov rcx, _buffer.size
    call number_to_string
    call print_string   ; вызываем функцию печати числа
    call print_line     ; печатаем переводс строки
    call exit

section '.number_to_string' executable
; | input
; | rax = number
; | rbx = buffer     
; | rcx = buffer.size
number_to_string:
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



section '.print_char' executable
; | input
; rax = char
print_char:
    
    push rbx
    push rcx
    push rdx
    push rax
                      ;  дял 64 битного режима 
    mov rax, 1         ;  1 - write вместо 4 в 32 битном режиме 
    mov rdi, 1         ;  stdout = 1 в rdi вместо 0 в rbx в 32 битном режиме  
    mov rsi, rsp       ;  rsp указывает на последнее значение в стеке т.е. на RAX ('W')    
    mov rdx, 1         ;  в rdx кладётся длина
    syscall
    pop rax
    pop rdx
    pop rcx
    pop rbx
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