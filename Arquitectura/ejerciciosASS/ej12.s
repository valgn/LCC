.data
list: .long 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 # 4 bytes
length: .byte 10

.text

suma:
    addl (%rsi), %edi
    addq $4, %rsi
    loop suma
    ret



.global main
main:
    xor %rcx, %rcx
    movb length, %cl
    xor %rdi, %rdi
    xor %rsi, %rsi
    leaq list, %rsi
    call suma


    ret
.section .note.GNU-stack
