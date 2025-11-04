/*
int calculo(char a, char b, char c, int d, char e, float f, double g, int h,
            int i, int j)
{
    return (a + c + d) * (i + j);
}

 */

.data
f: .float 1.0
g: .double 2.0

.text

calculo:
    pushq %rbp
    movq %rsp, %rbp

    movq %rdi, %rax
    addq %rdx, %rax
    addq %rcx, %rax

    movq 16(%rbp), %r10
    addq 24(%rbp), %r10

    imulq %r10

    
    movq %rbp, %rsp
    popq %rbp
ret


.global main
    main:
    /*  int e = 5;
        return calculo(1, 2, 3, 4, 5, 1.0, 2.0, 6, 7, 8); 
    */
    # Prologo
    pushq %rbp
    movq %rsp, %rbp
    
    movq $1, %rdi # a
    movq $2, %rsi # b
    movq $3, %rdx # c
    movq $4, %rcx # d
    movq $5, %r8 # e
    movss f(%rip), %xmm0 # f
    movsd g(%rip), %xmm1 # g
    movq $6, %r9 # h
    pushq $8 # j
    pushq $7 # i

    call calculo

    #Epilogo
    movq %rbp, %rsp
    popq %rbp

    ret

.section .note.GNU-stack

