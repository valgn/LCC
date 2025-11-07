.data
    cadena: .asciz "123456"
.text



.global busca

busca:
    # completar aqui
    # si el char est ́a en la cadena en rax debe quedar
    # la direcci ́on en donde se encontr ́o el char.
    # Si no, en rax debe quedar el valor -1
    
    cmp (%rdi), %sil
    je encontro
    incq %rdi
    cmpb $0, (%rdi)
    jne busca
    
    noencontro:
        xor %rax, %rax
        movq $0, %rax #  cambie de -1 a 0 porque 0 es NULL
        jmp fin 

    encontro:
        xor %rax, %rax
        movq %rdi, %rax
        jmp fin
    
    fin:
        ret

.section .note.GNU-stack
