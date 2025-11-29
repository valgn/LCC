.data
a: .word -1, -2, 0xff, 0xab
g: .quad 0x12345678
msg: .asciz "%d\n"

.text




.global main
main:
    movq $g, %rbx
    movq $3, %rcx
    xor %rax, %rax
    movl (%rbx, %rcx, 1), %eax
    
    ret



.section .note.GNU-stack

