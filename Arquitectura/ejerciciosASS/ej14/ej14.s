.text

# hola 3 2 3
.globl suma_cuadrados
# RDI = CANTIDAD DE ARGUMENTOS
# RSI = STRING DE NUMEROS [0] ES EL NOMBRE DEL PROGRAMA

suma_cuadrados:

    # Prologovich
    pushq %rbp
    movq %rsp, %rbp

    xor %rcx, %rcx
    xor %rdx, %rdx
    movq %rdi, %r13 # R13 = argc
    decq %r13

    cmpq $0, %r13
    je fin  # o return 0

    xor %rbx, %rbx # Rbx = 0
    
    addq $8, %rsi
    
    iterar:
    movq (%rsi), %rdi
    movq %rsi, %r12
    call atoi
    movq %r12, %rsi
    addq $8, %rsi
    imulq %rax
    movq %rax, %rdx
    addq %rdx, %rbx
    decq %r13
    jnz iterar 

    movq %rbx, %rax
    fin:
        # Epilogoide
        movq %rbp, %rsp
        popq %rbp
        ret


.section .note.GNU-stack
