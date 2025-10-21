.data
a: .word -1 # 0x404010 // 0x404011 = 0xffff
b: .byte 0xaa, 99  # 0x404012 = 0xaa, // 0x404013 = 0x63
c: .float 1.5 # 0x404014 = 0x3fc00000
str: .asciz "Hola mundo\n" # 0x404018 + 0x00000b = 0x404023 termina la cadena
.text

.global main

main:
leaq a, %rdx
movw (%rdx), %ax      # ax = 0xffff
movb 0x404013, %al    # al = 0x63
movl str, %esi        # esi = "aloH"

movl c, %edi          # edi = 0x3fc00000

movl %esi, (%rdx)
ret

.section .note.GNU-stack
