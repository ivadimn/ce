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
   
    mov rax, 571898928
    call print_number   ; вызываем функцию печати числа
    ;call print_line     ; печатаем переводс строки
    call exit
    
section '.string_to_number' executable
; | input
; rax = string
; | output
; rax = number
string_to_number:
    push rbx
    push rcx
    push rdx
    xor rcx, rcx
    xor rbx, rbx

    .next_iter:
        cmp [rax + rbx], byte 0
        je .next_step
        mov cl, [rax + rbx]
        sub cl, '0'
        push rcx        
        inc rbx
        jmp .next_iter
    .next_step:
        mov rcx, 1
        xor rax, rax
    .to_number:
        cmp rbx, 0
        je .close
        pop rdx
        imul rdx, rcx
        imul rcx, 10
        add rax, rdx
        dec rbx
        jmp .to_number
    .close:
        pop rdx
        pop rcx
        pop rbx    
        ret

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
    dec rsi
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
        je .null_char
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
    .null_char:
        mov rsi, rdx        
    .close:
        mov [rbx + rsi], byte 0
        pop rsi
        pop rdx
        pop rcx
        pop rbx       
        pop rax
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
    push rsi
    push rdi
    push rdx
    ;mov rax, '5'
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
    mov rax, 0x3c   ; - указание операционной системе что exit
    mov rdi, 0      ; - возвращаемое программорй значание return (0 - ошибок нет)
    syscall