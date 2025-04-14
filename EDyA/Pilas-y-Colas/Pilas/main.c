#include "arregloenteros.h"
#include "pilas.h"
#include <stdlib.h>
#include <stdio.h>


int main(){
    Pila pila = pila_crear(5);

    
    pila_apilar(pila, 2);
    pila_apilar(pila, 3);
    pila_apilar(pila, 4);
    pila_apilar(pila, 5);

    pila_desapilar(pila);
    pila_desapilar(pila);
    pila_desapilar(pila);
    pila_desapilar(pila);
    pila_desapilar(pila);
    pila_desapilar(pila);

    pila_apilar(pila,3);
    pila_apilar(pila,3);
    pila_apilar(pila,3);
    pila_apilar(pila,3);
    pila_apilar(pila,3);
    pila_apilar(pila,3);
    pila_apilar(pila,3);
    pila_apilar(pila,3);
    pila_apilar(pila,3);
    
    


    
    //printf("%d\n", pila_es_vacia(pila));

    pila_imprimir(pila);

    pila_destruir(pila);

    return 0;
}