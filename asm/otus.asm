format ELF64
public _start
extrn malloc

section '.data' writeable
    _buffer.size equ 32
    empty_str db 0x0
    int_format db "%ld ", 0x0
    _array dq 4, 8, 15, 16, 23, 42
    _length = ($ - _array) / 8

section '.bss' writeable
    _buffer rb _buffer.size
    _bss_char rb 1

section '.text' executable
_start:
    mov rbx, _length
    mov rax, [_array - 8 + rbx * 8]
    mov rbx, _buffer
    mov rcx, _buffer.size
    call number_to_string
    mov rax, _buffer
    call print_string   ; вызываем функцию печати строки
    ;call print_line     ; печатаем переводс строки
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
    push rsi
    mov rsi, rcx
    xor rcx, rcx
    
    .next_iter:
        push rbx
        mov rbx, 10
        xor rdx, rdx        ; в rdx помещается остаток отделения    
        div rbx 
        pop rbx                
        add rdx, '0'         ; в div делимое всегда берётся из rax     
        push rdx             ; сохраняем символ в стеке   
        inc rcx
        cmp rax, 0
        je .next_step
        jmp .next_iter
    .next_step:
        mov rdx, rcx
        xor rcx, rcx    
    .to_string:
        cmp rcx, rsi
        je .pop_iter
        cmp rcx, rdx
        je .close
        pop rax
        mov [rbx+rcx], rax
        inc rcx
        jmp .to_string
    .pop_iter:
        cmp rcx, rdx
        je .close
        pop rax
        inc rcx
        jmp .pop_iter
    .close:
        pop rsi
        pop rdx
        pop rcx
        pop rbx       
        pop rax
        ret

section '.print_char' executable
; | input
; rax = char
print_char:
    
    push rsi
    push rdi
    push rdx
    push rax
                       ;  дял 64 битного режима 
    mov rsi, rsp       ;  rsp указывает на последнее значение в стеке т.е. на RAX ('W')                       
    mov rax, 1         ;  1 - write вместо 4 в 32 битном режиме 
    mov rdi, 1         ;  stdout = 1 в rdi вместо 0 в rbx в 32 битном режиме  
    mov rdx, 1        ;  в rdx кладётся длина
    syscall
    pop rax
    pop rdx
    pop rdi
    pop rsi
    
    ret

section '.print_line' executable
print_line:
    push rax
    mov rax, 0xA
    call print_char
    pop rax
    ret    

section '.print_string' executable
; | input
; rax = адрес строки
print_string:
    push rax
    push rdx
    push rdi
    push rsi
    mov rsi, rax         ; адрес строки в rcx
    call length_string
    mov rdx, rax         ; длину строки в rdx
    mov rax, 1         ; указание ос что будет запись
    mov rdi, 1         ; в стандартный поток вывода (терминал)
    syscall
    call print_line
    pop rsi
    pop rdi
    pop rdx
    pop rax
    ret

section '.length_string' executable
; | input
; rax = адрес строки
; | output
; rax length
length_string:
    push rdx    ; сохраняем rdx
    xor rdx, rdx
    .next_iter:
        cmp [rax + rdx], byte 0
        je .close
        inc rdx
        jmp .next_iter
    .close:
        mov rax, rdx    ; сохраняем длину строки в raz
        pop rdx         ; востанавливаем rdx    
        ret

section '.exit' executable
exit:
    mov rax, 1 ; - указание операционной системе что exit
    mov rbx, 0 ; - возвращаемое программорй значание return (0 - ошибок нет)
    int 0x80
