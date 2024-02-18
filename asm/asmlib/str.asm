format ELF64

public string_to_number
public number_to_string
public length_string

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