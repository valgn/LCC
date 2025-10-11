.global main
main:
call ejercicio8


ret

ejercicio8:
movl $-1, %eax # Solo para este tama~no el mov pone en 0 la parte alta del

movl $2, %ecx
imull %ecx
shlq $32, %rdx
addq %rdx, %rax


xorq %rax, %rax
movw $-1, %ax
movw $2, %cx
mulw %cx

shll $16, %edx
addl %edx, %eax

ret


.section .note.GNU-stack
