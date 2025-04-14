#include <stdio.h>
#include <stdlib.h>
#include "gpilas.h"

GPila crear_gpila(){
    GPila pila = glist_crear();
    return pila;
}

void gpila_destruir(GPila pila, FuncionDestructora destroy){
    glist_destruir(pila, destroy);
}

void gpila_es_vacia(GPila pila){
    return(pila== NULL);

}

int pila_tope(GPila pila){
    return pila->lista->data;
}

void pila_apilar(GPila pila, int dato, FuncionCopia copy){
    glist_agregar_inicio(pila, dato, copy);
}


void pila_desapilar(GPila pila){
    GPila temp = pila;
    pila = pila->lista->next;
    free(temp);
}

void pila_imprimir(GPila pila, FuncionVisitante visit){
    glist_recorrer(pila, visit);
}
