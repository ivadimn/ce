format ELF64
public _start

extrn number_to_string
extrn print_string
extrn exit

section '.data' writeable
    strnum db "571345", 0
    _buffer.size equ 32

section '.bss' writeable
    _buffer rb _buffer.size
    _bss_char rb 1

section '.text' executable
_start:
    
    mov rax, 256
    mov rbx, _buffer
    mov rcx, _buffer.size
    call number_to_string
    mov rax, _buffer
    call print_string       
    call exit

