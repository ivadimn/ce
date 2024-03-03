format ELF64
public _start

include "asmlib/math.inc"
include "asmlib/fmt.inc"
include "asmlib/sys.inc"

section '.data' writeable
    fmt db "[%d]", 0
    msg db "hello", 0
    msg2 db "world", 0

section '.text' executable
_start:
    ;push 62526
    push 0xF43E
    mov rax, fmt
    call printf
    call print_line
    call exit

section '.printf' executable    
; | input:
; rax = format
; stack = values
printf:
    push rax
    push rbx
    mov rbx, 32
    .next_iter:
        cmp [rax], byte 0
        je .close
        cmp [rax], byte '%'
        je .special_char
        jmp .default_char
        .special_char:
            inc rax
            cmp [rax], byte 's'
            je .print_string
            cmp [rax], byte 'd'
            je .print_number
            cmp [rax], byte 'o'
            je .print_oct
            cmp [rax], byte 'x'
            je .print_hex
            cmp [rax], byte 'c'
            je .print_char
            cmp [rax], byte '%'
            je .print_persent
            jmp .next_step
        .print_string:
            push rax
            mov rax, [rsp + rbx]
            call print_string
            pop rax    
            jmp .shift_stack
        .print_number:
            push rax
            mov rax, [rsp + rbx]
            call print_number
            pop rax    
            jmp .shift_stack    
        .print_oct:
            push rax
            mov rax, [rsp + rbx]
            call print_oct
            pop rax    
            jmp .shift_stack       
        .print_hex:
            push rax
            mov rax, [rsp + rbx]
            call print_hex
            pop rax    
            jmp .shift_stack           
        .print_char:
            push rax
            mov rax, [rsp + rbx]
            call print_char
            pop rax    
            jmp .shift_stack        
         .print_persent:
            push rax
            mov rax, '%'
            call print_char
            pop rax
            jmp .next_step    
        .default_char:
            push rax
            mov rax, [rax]
            call print_char
            pop rax
            jmp .next_step
        .shift_stack:
            add rbx, 8    
        .next_step:
            inc rax            
            jmp .next_iter
    .close:
        pop rbx
        pop rax
        ret

section '.print_oct' executable
print_oct:
    push rax
    push rbx
    push rcx
    push rdx

    xor rcx, rcx
    mov rbx, 8
    push rax
    mov rax, '0'
    call print_char
    pop rax 
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

section '.print_hex' executable
print_hex:
    push rax
    push rbx
    push rcx
    push rdx

    xor rcx, rcx
    mov rbx, 16
    push rax
    mov rax, '0'
    call print_char
    mov rax, 'x'
    call print_char
    pop rax 
    
    .next_iter:
        xor rdx, rdx        ; в rdx помещается остаток отделения    
        div rbx             ; в div делимое всегда берётся из rax     
        cmp rdx, 10
        jl .is_digit
        jmp .is_alpha
        .is_digit:
            add rdx, '0'
            jmp .next_step
        .is_alpha:
            sub rdx, 10
            add rdx, 'A'
            jmp .next_step
        .next_step:
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
