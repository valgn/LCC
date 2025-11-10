.data
str1: .asciz "Estoy en foo!\n"
str2: .asciz "La cantidad de argumentos es %d\n"
str3: .asciz "El argumento %d es %s\n"

.text

.global main
main: 
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq   %rdi  ,-8(%rbp)
    movq   %rsi  ,-16(%rbp)
    call foo
    movq $str2, %rdi
    movq -8(%rbp), %rsi
    xor %rax, %rax
    call printf
    xorl %r12d, %r12d
    movq -16(%rbp), %rbx

bucle:
    movq  $str3  , %rdi
    movq   %r12 , %rsi
    movq  (%rbx,%r12,8), %rdx
    xorq %rax, %rax
    call printf
    incq %r12
    cmp  %r12,-8(%rbp)
    jne bucle
    movq %rbp, %rsp
    popq %rbp
    xorl %eax, %eax
    ret
# Fin de main
foo:
    pushq %rbp
    movq %rsp, %rbp
    movq $str1, %rdi
    xor %rax, %rax
    call printf
    movq %rbp, %rsp
    popq %rbp
    xorl %eax, %eax
    ret

.section .note.GNU-stack
