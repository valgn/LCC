.data
format: .asciz "%ld\n"
hexa: .asciz "0x%p\n"
quad: .asciz "%lld\n"
i: .quad 0xDEADBEEF
.text

.global main
main:

# Prologo
pushq %rbp
movq %rsp, %rbp

movq $format, %rdi # El primer argumento es el formato
movq $1234, %rsi # El valor a imprimir
xorq %rax, %rax # Cantidad de valores de punto flotante
call printf


// valor del registro rsp
movq $format, %rdi 
movq %rsp, %rsi 
xorq %rax, %rax 
call printf

// La direcci´on de la cadena de formato.
movq $format, %rdi 
movq $format, %rsi 
xorq %rax, %rax 
call printf

//  La direcci´on de la cadena de formato en hexadecimal.
movq $hexa, %rdi 
movq $format, %rsi 
xorq %rax, %rax 
call printf

// Imprimir el quad en el tope de la pila
movq $quad, %rdi 
movq (%rsp), %rsi
xorq %rax, %rax 
call printf

// Imprimir el quad ubicado en rsp+8
movq $quad, %rdi
movq 8(%rsp), %rsi
xorq %rax, %rax 
call printf

// El valor i
movq $quad, %rdi
movq i, %rsi
xorq %rax, %rax 
call printf

// La direccion de i
movq $format, %rdi
movq $i, %rsi
xorq %rax, %rax 
call printf



# Epilogo
movq %rbp, %rsp
popq %rbp

ret

.section .note.GNU-stack
