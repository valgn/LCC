#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

union FloatBits {
    float f;
    unsigned int u;
};

unsigned int is_one(unsigned int n, unsigned int b)
{
    // 10 = 00001010
    return n & 1ULL << b ? 1 : 0;
    // * hace un and entre el numero en binario y un binario que tiene todos 0 menos un 1 en la posicion que te interesa
}

void printbin(unsigned int n){
    for(int i = 31; i >= 0; i--){
        printf("%d", is_one(n,i));
    }

printf("\n");
}

unsigned int fraccion(union FloatBits f){
    unsigned int mask = 0x007fffff;
    return f.u & mask;
}

unsigned int exponente(union FloatBits f){
    unsigned int mask = 0x7F800000;
    return (f.u & mask) >> 23;
}



int main(){
    union FloatBits fb;
    fb.f = 6.225;
    unsigned int frac = fraccion(fb);
    unsigned int expo = exponente(fb);
    printbin(fb.u);
    printf("\n");
    printbin(frac);
    printf("\n");
    printbin(expo);
    printf("\n");
    float f = 6.225f;
    printf("Error: %.20f\n", 6.225 - fb.f);
}