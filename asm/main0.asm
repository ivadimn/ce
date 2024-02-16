format ELF64
public _start

section '.data' writeable
    new_line equ 0xA
    msg db "Hello, world!", new_line, 0
    ;len = $ - msg


section '.text' executable
_start:
    ;mov rax, 4         ; указание ос что будет запись
    ;mov rbx, 0         ; в стандартный поток вывода (терминал)
    ;mov rcx, msg       ; передаем адрес строкм
    ;mov rdx, len       ; передаем длину строки
    ;int 0x80
    mov rax, msg        ; помещаем в rax адрес строки
    call print_string   ; вызываем функцию печати строки
    mov rax, 'W'
    call print_char
    call print_line
    call exit

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
    mov rax, 60 ; - указание операционной системе что exit
    mov rdi, 0  ; - возвращаемое программорй значание return (0 - ошибок нет)
    syscall