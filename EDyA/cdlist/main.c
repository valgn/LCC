#include <stdio.h>
#include <stdlib.h>
#include "cdlist.h"
#include "cdlist.c"

int main(){
    CDList* lista = crearlista();
    agregar(lista, 3);
    agregar(lista, 2);
    agregar(lista, 1); 

    recorrer2(lista, imprimir_doble);
    
    return 0;
}