.text
.global main
main:
    movq $12, %rax
    cmp $10, %rax
    jg mayor

    menor:
        movq $-1, %rbx
        jmp fin

    mayor:
        movq $1, %rbx
        jmp fin


    fin:
        ret

.section .note.GNU-stack
