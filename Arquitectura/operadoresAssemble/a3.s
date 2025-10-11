.global main
main:
movl $0xAA, %eax
shll $24, %eax

movl $0xAA, %ebx

orl %ebx, %eax

ret


.section .note.GNU-stack
