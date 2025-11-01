/*
unsigned long fact1(unsigned long x){
if (x <= 1) {
return x;
}
return x * fact1(x - 1);
}
 */

.text
.globl fact1
fact1:
    # Prologo
    pushq %rbp
    movq %rsp, %rbp

    cmpq $1, %rdi
    jg recursion
    movq $1, %rax
    jmp fin



    recursion:
    pushq %rdi
    decq %rdi
    call fact1
    popq %rdi
    imulq %rdi

    fin:
    # Epilogo
    movq %rbp, %rsp
    popq %rbp
    ret

.section .note.GNU-stack
