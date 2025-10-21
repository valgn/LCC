#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int is_one(long long n, int b)
{
    // 10 = 00001010
    return n & 1ULL << b ? 1 : 0;
    // * hace un and entre el numero en binario y un binario que tiene todos 0 menos un 1 en la posicion que te interesa
}

void printbin(long long n){
    for(int i = 3; i >= 0; i--){
        printf("%d", is_one(n,i));
        if(i % 8 == 0 && i != 0){
            printf(" ");
    }
}
printf("\n");
}
 /* 
 ? 17) Implemente una funci´on en lenguaje C que tome tres par´ametros a, b y c y que rote
 ? los valores de las variables de manera que al finalizar la funci´on el valor de a se encuentre
 ? en b, el valor de b en c y el de c en a. Evitar utilizar variables auxiliares.
 ? Ayuda: Tener en cuenta las propiedades del operador XOR. Se puede pensar primero
 ? en intercambiar dos variables
 */

void cambiar(long* a, long* b, long* c){
    *a = *a^*b;
    *b = *a^*b;
    *a = *a^*b;
    // ? a ahora es b y b es a
    /*
    b
    a
    c
    */
   *b = *b^*c;
   *c = *b^*c;
   *b = *b^*c;
}   


int main(int argc, char* argv[])
{
    // long a = 1;
    // long b = 2;
    // long i = 4;
    
    // printbin(a); printbin(b); printbin(i);
    // cambiar(&a,&b,&i);
    // printf("\n");
    // printbin(a); printbin(b); printbin(i);

    int codigo = atoi(argv[1]);
    char* mensaje = argv[2];

    for(int i = 0; i < strlen(mensaje); i++){
        char codificado = mensaje[i] ^ codigo;
        printf("%c", codificado);
    }
    printf("\n");
    return 0;

    
}