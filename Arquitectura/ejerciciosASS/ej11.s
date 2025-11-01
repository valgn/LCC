.data
a: .byte 1, 2, 3, 4, 5
l: .byte 5

.text

cuerpo:
    # prologo
    pushq %rbp
    movq %rsp, %rbp

    movb l, %cl

    # Apilar
    apilar:
        pushq (%rbx)
        incq %rbx
        loop apilar

    xor %rcx, %rcx
    movb l, %cl
    leaq a, %rbx

    #
    desapilar:
        popq (%rbx)
        incq %rbx
 
        loop desapilar


    # epilogo
    movq %rbp, %rsp
    popq %rbp
    
    ret


.global main
    main:
    xor %rcx, %rcx
    xor %rbx, %rbx
    leaq a, %rbx
    call cuerpo
    ret




.section .note.GNU-stack
