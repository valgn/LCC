.data
list:   .long 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
length: .byte 10

msg_suma:     .asciz "Suma: %d\n"
msg_mayor:    .asciz "Mayor: %d\n"
msg_menor:    .asciz "Menor: %d\n"
msg_promedio: .asciz "Promedio: %d\n"



.text
.global main
.extern printf

#######################################
# suma: acumula todos los valores del arreglo
# Entrada: rsi -> inicio del arreglo, rcx -> cantidad de elementos
# Salida:  eax -> suma total
#######################################
suma:
    xor %eax, %eax          # acumulador = 0
bucle_suma:
    addl (%rsi), %eax       # eax += *rsi
    addq $4, %rsi           # avanzar al siguiente elemento
    dec %rcx
    jnz bucle_suma
    ret

#######################################
# mayor: busca el máximo
# Entrada: rsi -> inicio del arreglo, rcx -> cantidad de elementos
# Salida:  eax -> valor máximo
#######################################
mayor:
    movl (%rsi), %eax       # eax = primer elemento
    addq $4, %rsi
    dec %rcx
bucle_mayor:
    cmpl %eax, (%rsi)
    jge skip_max
    movl (%rsi), %eax
skip_max:
    addq $4, %rsi
    dec %rcx
    jnz bucle_mayor
    ret

#######################################
# menor: busca el mínimo
# Entrada: rsi -> inicio del arreglo, rcx -> cantidad de elementos
# Salida:  eax -> valor mínimo
#######################################
menor:
    movl (%rsi), %eax
    addq $4, %rsi
    dec %rcx
bucle_menor:
    cmpl (%rsi), %eax
    jge skip_min
    movl (%rsi), %eax
skip_min:
    addq $4, %rsi
    dec %rcx
    jnz bucle_menor
    ret

#######################################
# main
#######################################
main:
    # ---------- SUMA ----------
    movzbq length, %rcx
    leaq list, %rsi
    call suma
    movl %eax, %r8d           # guardar suma en r8d
    movl %eax, %esi           # arg1 = valor
    leaq msg_suma(%rip), %rdi
    xor %eax, %eax
    call printf

    # ---------- MAYOR ----------
    movzbq length, %rcx
    leaq list, %rsi
    call mayor
    movl %eax, %r9d           # guardar mayor
    movl %eax, %esi
    leaq msg_mayor(%rip), %rdi
    xor %eax, %eax
    call printf

    # ---------- MENOR ----------
    movzbq length, %rcx
    leaq list, %rsi
    call menor
    movl %eax, %r10d          # guardar menor
    movl %eax, %esi
    leaq msg_menor(%rip), %rdi
    xor %eax, %eax
    call printf

    # ---------- PROMEDIO ----------
    movl %r8d, %eax           # eax = suma
    xor %edx, %edx
    movzbq length, %rcx
    div %ecx                  # eax = suma / length
    movl %eax, %esi
    leaq msg_promedio(%rip), %rdi
    xor %eax, %eax
    call printf

    ret

.section .note.GNU-stack
