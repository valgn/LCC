.data
pocosArgumentos: .asciz "La cantidad de argumentos es insuficiente.\n"
printSuma: .asciz "Cantidad de argumentos: %d, suma de los argumentos: %d\n"

.text
.global main
// set args [numeros]

main:
    pushq %rbp
    movq %rsp, %rbp

    cmp $1, %rdi
    jne hacerSuma
    
    movq $pocosArgumentos, %rdi
    xor %rax,%rax
    call printf
    jmp fin

    hacerSuma:
        movq %rdi, %r13
        pushq %rdi
        decq %r13
        addq $8, %rsi
        xor %rbx, %rbx
        
        iterar:
            movq (%rsi), %rdi
            pushq %rsi
            call atoi
            popq %rsi
            addq $8, %rsi
            addq %rax, %rbx
            decq %r13
            jnz iterar
        movq %rbx, %rdx
        popq %rsi
        decq %rsi
        xor %rax, %rax
        movq $printSuma, %rdi
        call printf

    
    fin:
        movq %rbp, %rsp
        popq %rbp
        ret

.section .note.GNU-stack

