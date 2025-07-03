#include "funciones.h"
#include "parser.h"
#include <stdio.h>

int main(){
    char linea[256];
    while(1){
        printf(">>> ");
        fgets(linea, sizeof(linea) ,stdin);
        procesar_linea(linea);
    }
    
    return 0;
}