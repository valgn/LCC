/* 
int fuerza_bruta2(const char *S, const char *s, unsigned lS, unsigned ls)
{
    
    unsigned int max = lS - ls;
    for(int i = 0; i <= max; i++){
        char* inicio = busca(S+i, s[0], lS-i);
        if(inicio == NULL){
            return -1;
        }
        int res = compara(inicio, s, ls);
        
        unsigned int pos = inicio-S;
        if(res == 1){
            return pos;
        }
        i = pos;
        
    
    }

    return -1;
}
*/

.data
S: .asciz "51224124123"
s: .asciz "123"
lS: .int 11
ls: .int 3
printear: .asciz "%d\n"

.text

busca:

    cmpb %sil, (%rdi)
    je encontro
    incq %rdi
    cmpb $0, (%rdi)
    jne busca
    
    noencontro:
        xor %rax, %rax
        movq $0, %rax #  cambie de -1 a 0 porque 0 es NULL
        jmp fin 

    encontro:
        xor %rax, %rax
        movq %rdi, %rax
        jmp fin
    
    fin:
        ret


compara:

    movb (%rsi), %al
    cmpb %al, (%rdi)
    jne distintas
    incq %rsi
    incq %rdi
    decl %edx
    jnz compara

    movq $1, %rax
    jmp fin2

    distintas:
        xor %rax, %rax

    fin2:
        ret
    

.global main
main:
    movq $S, %rdi
    movq $s, %rsi
    movl lS, %edx
    movl ls, %ecx

    movq lS, %r12
    subq ls, %r12 # maximo lugar en donde puede haber una aparicion de la cadena
    xorq %r13, %r13

    movq %rdi, %r15 # direccion de S
    movq %rsi, %rbx
    iteracion:
        movq %r15, %rdi
        addq %r13, %rdi
        movq %rbx, %rsi

        movb (%rsi), %sil
        movl lS, %r10d
        subl %r13d, %r10d
        movl %r10d, %edx
        call busca

        cmp $0, %rax
        je nulo

        movq %rax, %r14
        subq %r15, %r14 # pos ---
        movq %rax, %rdi
        movq $s, %rsi
        movl ls, %edx
        call compara

        cmp $1,%rax
        je retornar

        movq %r14, %r13
        incq %r13
        cmp %r13, %r12
        jle iteracion

    nulo:
        movl $-1, %eax
        jmp print

    retornar:
        movq %r14, %rax
        jmp print
        
    print:
        movq $printear, %rdi
        movq %rax, %rsi
        xor %rax, %rax
        pushq $1
        call printf 
        popq %rax
        ret

.section .note.GNU-stack
