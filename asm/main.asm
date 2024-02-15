format ELF64
public _start

section '.data' writeable
    _buffer.size equ 32

section '.bss' executable
    _buffer rb _buffer.size
    _bss_char rb 1

section '.text' executable
_start:
   
    mov rax, 571
    mov rbx, _buffer
    mov rcx, _buffer.size
    call number_to_string
    call print_string   ; вызываем функцию печати строки
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
    mov rbx, 1         ; в стандартный поток вывода (терминал)
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