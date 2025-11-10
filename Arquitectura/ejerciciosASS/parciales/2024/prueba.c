#include <stdio.h>

long producto(long a, long b){
    return a*b;
}

int y = 6;

int main(){
    long x = 4;
    printf("%ld\n", producto(x,y));
    return 0;
}