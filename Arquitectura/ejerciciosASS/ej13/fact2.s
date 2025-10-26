/*
unsigned long fact2(unsigned long x)
{
unsigned long acc = 1;
    for (; x > 1; x--) {
        acc *= x;
    }
return acc;
}
 */

.text

.globl fact2
fact2:
    # Prologo
    pushq %rbp
    movq %rsp, %rbp

    movq $1, %rax

    iterar:
        cmpl $1, %edi
        jle terminar
        imulq %rdi
        decq %rdi
        jmp iterar
    
    terminar:
        # Epilogo
        movq %rbp, %rsp
        popq %rbp
        ret
        


.section .note.GNU-stack
