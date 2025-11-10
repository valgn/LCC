.data
str1: .asciz "El promedio de las notas es %lf\n"
str2: .asciz "Ingrese aunque sea una nota\n"
a: .int 0xdeadbeef

.text
.global main
main:
    pushq %rbp
    movq %rsp, %rbp
    movq a, %rax
    # En rdi = argc, rsi = argv
    cmp $2, %rdi
    jl pocosArgumentos

    pushq %rdi
    movl %edi, %r12d
    decl %r12d
    addq $8, %rsi
    xor %rbx, %rbx

    bucle:
        movq (%rsi), %rdi
        pushq %rsi
        call atoi
        popq %rsi
        addl %eax, %ebx
        addq $8, %rsi
        decl %r12d
        jnz bucle
    
    # rbx va a tener la suma de todas las notas
    cvtsi2sd %rbx, %xmm0 # suma
    popq %rdi
    decq %rdi
    cvtsi2sd %rdi, %xmm1 # cantidad
    divsd %xmm1, %xmm0
    movq $1, %rax
    leaq str1, %rdi
    call printf
    xor %rax, %rax
    jmp fin

    pocosArgumentos:
        xor %rax, %rax
        leaq str2, %rdi
        call printf
        movq $1, %rax
        jmp fin

    fin:
        movq %rbp, %rsp
        popq %rbp
        ret

       

.section .note.GNU-stack

