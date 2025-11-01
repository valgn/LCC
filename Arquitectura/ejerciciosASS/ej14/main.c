#include <stdio.h>

unsigned long suma_cuadrados(int argc, char* argv[]);

int main(int argc, char *argv[]){

    if(argc <= 1){
        printf("Pone aunque sea dos argumentos.\n");
        return 0;
    }

    unsigned long resultado = suma_cuadrados(argc, argv);
    printf("Cantidad de argumentos ingresados: %d, La suma es: %lu.\n",argc-1, resultado);

    return 0;
}