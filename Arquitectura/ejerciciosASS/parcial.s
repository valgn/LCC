.data
b: .float 2.3
str1: .asciz "El valor escalado es %lf\n"

.text
.global main
main:
    pushq %rbp
    movq %rsp, %rbp

    movq 8(%rsi), %rdi
    call atoi
    movss b, %xmm0
    movq %rax, %rdi

    call scale
    cvttsd2si %xmm0, %eax
    leave
    ret


scale:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq $8 , -16(%rbp)
    movq %rdi, -8(%rbp)
    cvtsi2ss -8(%rbp), %xmm1
    mulss %xmm1, %xmm0
    cvtsi2ss -16(%rbp), %xmm1
    addss %xmm1, %xmm0
    cvtss2sd %xmm0, %xmm0
    cvttsd2si %xmm0, %r12d
    movl %r12d, -4(%rbp)
    call imprime
    movl -4(%rbp), %r10d
    cvtsi2sd %r10d, %xmm0

    movq %rbp, %rsp
    popq %rbp
    ret

imprime:
    pushq %rbp
    movq %rsp, %rbp
    movq $1, %rax
    leaq str1, %rdi
    call printf
    xorq %rax, %rax
    movq %rbp, %rsp
    popq %rbp
    ret

.section .note.GNU-stack


