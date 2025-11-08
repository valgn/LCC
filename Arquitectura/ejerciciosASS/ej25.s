.data
a: .float 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.10, 0.11, 0.12, 0.13, 0.15000
b: .float 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,0.10,0.11, 0.12, 0.13, 0.14, 0.15000
len: .int 14

.text

simd:
    pushq %rbp
    movq %rsp, %rbp

    xor %rax, %rax
    movq %rdx, %rax
    xor %rdx, %rdx
    movq $4, %rbx
    idivq %rbx # queda en rax la cantidad de bloques de 4, en rdx queda el resto (1, 2 ,3)
    xorps %xmm0, %xmm0
    xorps %xmm1, %xmm1

    sumar4:     
        movups (%rdi), %xmm0
        movups (%rsi), %xmm1

        addps %xmm1, %xmm0
        movups %xmm0, (%rdi)
        
        addq $16, %rdi
        addq $16, %rsi


        decq %rax
        jnz sumar4

    cmp $0, %rdx
    je fin

    sumarescalares: 
        movss (%rdi), %xmm0
        movss (%rsi), %xmm1
        addss %xmm1, %xmm0
        movss %xmm0, (%rdi)
        addq $4, %rdi
        addq $4, %rsi
        decq %rdx
        jnz sumarescalares     
        jmp fin

    fin: 
        # Epilogo
        movq %rbp, %rsp
        popq %rbp
        ret



.global main
main:
    # Prologo
    pushq %rbp
    movq %rsp, %rbp

    movq $a, %rdi
    movq $b, %rsi
    movq len, %rdx

    call simd


    # Epilogo
    movq %rbp, %rsp
    popq %rbp

ret

.section .note.GNU-stack
