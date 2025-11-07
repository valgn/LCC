.data
    cadenalarga: .asciz "123456"
    cadenacorta: .asciz "124" #=>1 , 124=>0, 456=>0
.text



.global compara
    compara:
    # retorna 1 si las cadenas son iguales en los primeros %edx caracteres
    # retorna 0 si son distintas
    movb (%rsi), %al
    cmpb %al, (%rdi)
    jne distintas
    incq %rsi
    incq %rdi
    decl %edx
    jnz compara

    movq $1, %rax
    jmp fin

    distintas:
        xor %rax, %rax

    fin:
        ret
    

.section .note.GNU-stack
