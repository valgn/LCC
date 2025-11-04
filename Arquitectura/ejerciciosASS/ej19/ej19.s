.data
a: .int 0x7fffffff
b: .int 1
cadena: .asciz "0x %x 0x %x\n"

.text
foo2:
    pushq %rbp
    movq %rsp, %rbp

    movl %edi, %eax
    addl %esi, %eax
    movl %eax, a


    movq %rbp, %rsp
    popq %rbp
ret



foo1:
    pushq %rbp
    movq %rsp, %rbp

    shll $31, %esi
    addl %edi, %esi
    movl %esi, %eax

    movl %eax, a

    movq %rbp, %rsp
    popq %rbp
ret

.global main

main:
    pushq %rbp
    movq %rsp, %rbp

    movl a, %edi
    movl b, %esi

    call foo2

    movl %eax, %r11d

    movl a, %edi
    movl b, %esi

    call foo1
    movq $cadena, %rdi
    movl %eax, %esi
    movl %r11d, %edx
    xor %rax, %rax
    call printf

    

    movq %rbp, %rsp
    popq %rbp
ret

.section .note.GNU-stack
