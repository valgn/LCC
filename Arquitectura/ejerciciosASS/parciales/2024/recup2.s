.data
y: .int 6
str1: .asciz "%d\n"

.text
.global main
main:
    pushq %rbp
    movq %rsp, %rbp
    movl $4, %edi
    movl y, %esi
    call producto
    movl %eax, %esi
    leaq str1, %rdi
    xor %rax, %rax
    call printf
    xor %rax, %rax
    movq %rbp, %rsp
    popq %rbp
    ret



producto:
    pushq %rbp
    movq %rsp, %rbp

    movl %edi, -4(%rbp)
    movl %esi, -8(%rbp)
    
    movl -4(%rbp), %eax
    imull -8(%rbp), %eax

    popq %rbp
    ret

.section .note.GNU-stack
