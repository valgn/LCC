#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned campero(unsigned a, unsigned b){  // b = 0100
    // b = 0
    // b = 1
    // b es par
    //b es impar
    int k = 0;

    while(b > 0){
        if(b & 1){
            k += a;
        }
        // duplicas a
        a<<=1;
        //dividimos b
        b>>=1;

    }
    return k;

}


int main(){
    unsigned a = 3;
    unsigned b = 5;
    printf("%d\n", campero(a,b));
    return 0;
}