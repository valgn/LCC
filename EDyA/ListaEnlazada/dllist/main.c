#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"
#include "practica.c"


int main() {
    DList* lista = crearlista();
    lista = meterinicio(lista, 10);
    lista = meterinicio(lista, 20);
    lista = meterinicio(lista, 30);

    recorrer_lista(lista, 1);

    return 0;
}