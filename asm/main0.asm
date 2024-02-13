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

    call exit

section '.print_string' executable
; | input
; rax = адрес строки
print_string:
    push rax
    push rbx
    push rcx
    push rdx
    mov rcx, rax         ; адрес строки в rcx
    call length_string
    mov rdx, rax         ; длину строки в rdx
    mov rax, 4         ; указание ос что будет запись
    mov rbx, 0         ; в стандартный поток вывода (терминал)
    int 0x80
    pop rdx
    pop rcx
    pop rbx
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