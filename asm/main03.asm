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
   
    mov rax, 50        ; помещаем в rax символ
    mov rbx, 15
    call gcd
    call print_number
    call print_line     ; печатаем переводс строки
    call exit

section '.gcd' executable
; | input:
; rax = number 1
; rbx = number 2
; | output
; rax number
gcd:
    push rbx
    push rdx
    .next_iter:
        cmp rbx, 0
        je .close
        xor rdx, rdx
        div rbx
        push rbx
        mov rbx, rdx
        pop rax
        jmp .next_iter
    .close:
        pop rdx
        pop rbx
        ret




section '.fibonacci' executable
; | input:
; rax = number
; | output
; rax number
fibonacci:
    push rbx
    push rcx
    mov rbx, 0
    mov rcx, 1
    cmp rax, 0
    je .next_step
    .next_iter:
        cmp rax, 1
        jle .close
        push rcx
        add rcx, rbx
        pop rbx
        dec rax    
        jmp .next_iter
    .next_step:
        xor rcx, rcx    
    .close:
        mov rax, rcx
        pop rcx
        pop rbx
        ret


section '.factorial' executable
; | input:
; rax = number
; | output
; rax number
factorial:
    push rbx

    mov rbx, rax
    mov rax, 1
    .next_iter:
        cmp rbx, 1
        jle .close
        mul rbx
        dec rbx
        jmp .next_iter
    .close: 
        pop rbx   
    ret


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


section '.print_char32' executable
; | input
; rax = char
print_char32:
    push rax
    push rbx
    push rcx
    push rdx

    mov [_bss_char], al

    mov rax, 4
    mov rbx, 1
    mov rcx, _bss_char
    mov rdx, 1
    int 0x80
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
    mov rax, 0x3c   ; - указание операционной системе что exit
    mov rdi, 0      ; - возвращаемое программорй значание return (0 - ошибок нет)
    syscall