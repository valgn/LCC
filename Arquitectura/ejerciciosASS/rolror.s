.global main
main:

movq $0x00000000FFFFEFFF, %rax
xorq %rbx, %rbx
movq $64, %rcx

contar:
rorq $1, %rax
adcq $0, %rbx
loop contar



ret


.section .note.GNU-stack
