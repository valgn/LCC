.data
list: .long 10, 20, 30, 1, 50, 60, 70, 80, 100, 90 # 4 bytes
length: .byte 10

msg_suma:     .asciz "Suma: %d\n"
msg_mayor:    .asciz "Mayor: %d\n"
msg_menor:    .asciz "Menor: %d\n"
msg_promedio: .asciz "Promedio: %d\n"

.text

/*
    # Prologo
    pushq %rbp
    movq %rsp, %rbp

    # EPILOGO
    movq %rbp, %rsp
    popq %rbp

 */



mayor:
    # Prologo
    pushq %rbp
    movq %rsp, %rbp
    
    mayor_loop:
        cmpl %edx, (%rsi)
    
        jle nohagonada
        movl (%rsi), %edx

    nohagonada:
        addq $4, %rsi
        loop mayor_loop
    

    
    # Epilogo
    movq %rbp, %rsp
    popq %rbp

ret

menor:
    # Prologo
    pushq %rbp
    movq %rsp, %rbp

    menor_loop:
        cmpl %eax, (%rsi)
    
        jg nohagonada2
        movl (%rsi), %eax
    nohagonada2:
    addq $4, %rsi
    loop menor_loop

    # EPILOGO
    movq %rbp, %rsp
    popq %rbp
    ret



suma:
    # Prologo
    pushq %rbp
    movq %rsp, %rbp
    
    suma_loop:
        addl (%rsi), %eax
        addq $4, %rsi
        loop suma_loop

    # EPILOGO
    movq %rbp, %rsp
    popq %rbp
    ret

.global main
main:

    
    # Prologo
    pushq %rbp
    movq %rsp, %rbp

    xor %rcx, %rcx
    movb length, %cl          # rcx = cantidad de elementos
    movl list, %edx           # edx = primer elemento (valor inicial del máximo)
    leaq list+4, %rsi         # rsi = apunta al segundo elemento
    decq %rcx                 # rcx = elementos restantes por comparar

    call mayor                # calcula el máximo, devuelve en edx
    
    # Printeamos, preparamos registros
    movl %edx, %r8d # Preservamos el valor de rdx por las dudas
    xor %rax, %rax  # Hay que hacer 0 a rax
    movl %edx, %esi # Lo que imprime en el %d
    leaq msg_mayor, %rdi # Donde arranca la cadena “Calculá la dirección de msg_suma en base al valor actual de %rip y guardala en %rdi”.
    call printf
    
    leaq list+4, %rsi
    xor %rcx, %rcx
    movb length, %cl
    # Guardamos el menor en rax
    xor %rax, %rax
    movl list, %eax
    decq %rcx

    call menor # Calcula el menor 

    # Printeamos, preparamos registros
    movl %eax, %r9d # Preservamos el valor de rdx por las dudas
    xor %rax, %rax  # Hay que hacer 0 a rax
    movl %r9d, %esi # Lo que imprime en el %d
    leaq msg_menor(%rip), %rdi # Donde arranca la cadena “Calculá la dirección de msg_suma en base al valor actual de %rip y guardala en %rdi”.
    call printf
    

    

    # Promedio
    xor %rcx, %rcx
    xor %rax, %rax
    leaq list, %rsi
    movb length, %cl
    call suma

    # Printeamos, preparamos registros
    movl %eax, %r12d # Preservamos el valor de rdx por las dudas
    xor %rax, %rax  # Hay que hacer 0 a rax
    movl %r12d, %esi # Lo que imprime en el %d
    leaq msg_suma(%rip), %rdi # Donde arranca la cadena “Calculá la dirección de msg_suma en base al valor actual de %rip y guardala en %rdi”.
    call printf
    


    xor %rcx, %rcx
    xor %rdx, %rdx
    movb length, %cl
    movl %r12d, %eax
    idivq %rcx

    # Printeamos, preparamos registros
    movl %eax, %r11d # Valor de division
    xor %rax, %rax  # Hay que hacer 0 a rax
    movl %r11d, %esi # Lo que imprime en el %d
    leaq msg_promedio(%rip), %rdi # Donde arranca la cadena “Calculá la dirección de msg_suma en base al valor actual de %rip y guardala en %rdi”.
    call printf

    # EPILOGO
    movq %rbp, %rsp
    popq %rbp

    ret


.section .note.GNU-stack
