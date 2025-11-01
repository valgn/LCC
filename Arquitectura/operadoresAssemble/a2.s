.global main
main:
movl $-1, %eax


movl $-1, %ebx
shll $8, %ebx 

andl %ebx, %eax
ret
// ahora veo q es al pedo xd pero lo dejo asi

.section .note.GNU-stack
