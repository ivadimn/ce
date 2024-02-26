format ELF64

public print_number
public print_string
public print_char
public print_line

extrn length_string

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

section '.print_char' executable
; | input
; rax = char
print_char:
    
    push rax
    push rbx
    push rcx
    push rdx
    push rdi
    push rsi
    push rax

    mov rsi, rsp       ;  rsp указывает на последнее значение в стеке т.е. на RAX ('W')                       
    mov rax, 1         ;  1 - write вместо 4 в 32 битном режиме 
    mov rdi, 1         ;  stdout = 1 в rdi вместо 0 в rbx в 32 битном режиме  
    mov rdx, 1        ;  в rdx кладётся длина
    syscall

    pop rax
    pop rsi
    pop rdi
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