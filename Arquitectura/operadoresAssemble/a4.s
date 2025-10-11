.global main
main:
movl $-1, %eax


movl $1, %ebx
shll $8, %ebx
notl %ebx 

andl %ebx, %eax
ret

// la practica me pide q haga esto pero me parece al pedo, ya hacerle el not a ebx te queda el numero que pide
.section .note.GNU-stack
