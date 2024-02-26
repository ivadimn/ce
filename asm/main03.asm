format ELF64
public _start

section '.data' writeable
    strnum db "571738", 0
    _buffer.size equ 32

section '.bss' writeable
    _buffer rb _buffer.size
    _bss_char rb 1

section '.text' executable
_start:
   
    mov rax, 321
    call print_number   ; вызываем функцию печати числа
    mov rax, 'G'
    call print_char   ; вызываем функцию печати числа
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

section '.print_char' executable
; | input
; rax = char
print_char:
    
    push rax
    push rsi
    push rdi
    push rdx
    
    push rax
                       ;  дял 64 битного режима 
    mov rsi, rsp       ;  rsp указывает на последнее значение в стеке т.е. на RAX ('W')                       
    mov rax, 1         ;  1 - write вместо 4 в 32 битном режиме 
    mov rdx, 1         ;  stdout = 1 в rdx вместо 0 в rbx в 32 битном режиме  
    mov rdi, 1        ;  в rd1 кладётся длина
    syscall

    pop rax

    pop rdx
    pop rdi
    pop rsi
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
    mov rax, 0x3c   ; - указание операционной системе что exit
    mov rdi, 0      ; - возвращаемое программорй значание return (0 - ошибок нет)
    syscall