#include <stdio.h>
#include <math.h>

union FloatBit{
    float f;
    unsigned int u;
};
typedef union FloatBit FloatBit;

int myisnan(FloatBit f){
    return f.f != f.f ? 1 : 0; // hice primero este que es la consigna b xd
}

int myisnan2(FloatBit f){
    unsigned int expo = (f.u >> 23) & 0xFF;
    unsigned int mantisa = f.u & 0x7FFFFF;
    return (expo == 0xFF) && (mantisa != 0); 
}


int main(void)
{
    float g = 0.0;
    float f = 0.0 / g;
    FloatBit f2;
    f2.f = f;
    printf("f: %f\n", f);
    // ADVERTENCIA : ‘NAN ’ es una extensi ´on de GCC .
    if (f == NAN)
    {
        printf("Es NAN \n");
    }
    if (isnan(f))
    {
        printf(" isNaN dice que si\n");
    }
    if(myisnan(f2)){
        printf("Funciona la verga esta.\n");
    }
    if(myisnan2(f2)){
        printf("Este tambien funca.\n");
    }
    return 0;
}
