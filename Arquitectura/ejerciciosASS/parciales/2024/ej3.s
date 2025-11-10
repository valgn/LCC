.data
arreglo: .double 1.0, 2.0, 3.0, 4.0
cantidad: .long 4
str: .asciz "La promedio es %lf, el suma es %lf.\n"

.text
suma_promedio:
    pushq %rbp
    movq %rsp, %rbp

    movq %rsi, %r12
    xorps %xmm0, %xmm0
    suma:
        addsd (%rdi), %xmm0
        addq $8,%rdi
        decq %r12
        jnz suma
        movsd %xmm0, %xmm1
        cvtsi2sdq %rsi, %xmm2
        divsd %xmm2,%xmm0
        movq %rbp, %rbp
        popq %rbp
        ret








.global main
main:
    pushq %rbp
    movq %rsp, %rbp

    movq $arreglo, %rdi
    movl cantidad, %esi
    call suma_promedio
    movq $str, %rdi
    movq $2, %rax
    call printf
    
    movq %rbp, %rbp
    popq %rbp
    xor %rax, %rax

    ret

.section .note.GNU-stack

