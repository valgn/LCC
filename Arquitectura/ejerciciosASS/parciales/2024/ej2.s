.data
a: .byte 1,2,3,4,5
b: .quad 0x1122334455667788
str: .asciz "Hola mundo\n"

.text
.global main
main:
    movq b, %rax
    movq $a, %rbx
    movl $4, %ecx
    movb (%rbx, %rcx, 1), %ah
    movl 0x5(%rbx), %eax
    lea b, %rbx
    movl b(,%rcx,1), %eax
    mulb %cl
    ret

.section .note.GNU-stack
