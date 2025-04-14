#include <stdio.h>
#include <stdlib.h>
#include "pilas.h"

Pila pila_crear(int capacidad){
    ArregloEnteros* arreglo = arreglo_enteros_crear(capacidad);
    Pila pila = malloc(sizeof(Pila));
    pila->arr = arreglo;
    pila->ultimo = -1;
    return pila;
}

void pila_destruir(Pila pila){
    free(pila->arr);
    free(pila);
}

int pila_es_vacia(Pila pila){
    return (pila->ultimo == -1);
}

int pila_tope(Pila pila){
    return pila->ultimo;
}

void pila_apilar(Pila pila, int elemento){
    if(pila->ultimo == pila->arr->capacidad - 1){
        arreglo_enteros_ajustar(pila->arr, (pila->arr->capacidad * 2));
    }
    int ultimo = pila->ultimo;
    pila->arr->direccion[ultimo + 1] = elemento; 
    pila->ultimo = ultimo + 1;
}

void pila_desapilar(Pila pila){
    if(!pila_es_vacia(pila)){
        pila->ultimo= pila->ultimo-1; 
    }

    
}

void pila_imprimir(Pila pila){
    for (int i = 0; i <= pila->ultimo; i++)
    {
        printf("%d, ", pila->arr->direccion[i]);
    }
    
}

