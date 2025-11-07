// void sum(float *a, float *b, int len); Implementar esto
// que suma dos arreglos de flotantes de longitud len dejando el resultado en a. Utilice instruciones
// escalares (i.e.: addss).
.data
a: .float 1.0, 2.0
b: .float 2.0, 3.0
len: .int 2

.text
.global main
main:
    pushq %rbp
    movq %rsp, %rbp

    movq $a, %rdi
    movq $b, %rsi
    movl len, %ecx         

sumar:
    movss (%rdi), %xmm0
    movss (%rsi), %xmm1
    addss %xmm1, %xmm0
    movss %xmm0, (%rdi)
    addq $4, %rdi
    addq $4, %rsi
    decq %rcx
    jnz sumar

    movq %rbp, %rsp
    popq %rbp
    ret

.section .note.GNU-stack
