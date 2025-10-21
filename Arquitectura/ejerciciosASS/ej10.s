.data
    a: .long 1, 2, 3, 4 # 0x404018 = 1 , 0x40401c = 2, 0x404020 = 3, 0x404024 = 4
    g: .quad 0x1122334455667788 # 0x404028
    msg: .asciz "Imprime %c\n" # 0x404030 - 0x40403b
.text
.global main
    main:
    subq $8, %rsp # rsp = 0x7fffffffebc8 - 8 =  0x7fffffffebc0
    movq $g, %rax # rax = 0x404028
    movl g+4, %eax # rax = 0x11223344
    movq $3, %rbx # rbx = 3
    movb a(, %rbx,4), %al # rax = 0 + 3 * 4 + 1 = 0x404024
    leaq msg, %rdi # rdi = 0x404030
    movb -1( %rdi, %rbx, 2), %sil # sil = 0x404036 = 'e'
    xorl %eax, %eax # rax = 0x0
    call printf # rax = 1
    addq $8, %rsp # rsp = 0x7fffffffebc0
    ret

.section .note.GNU-stack
