#include <stdio.h>
#include <stdlib.h>
#include "gpilas.h"

GPila crear_gpila(){
    GPila pila = malloc(sizeof(struct _GPilas));
    pila->lista = glist_crear();
    return pila;
}

void gpila_destruir(GPila pila, FuncionDestructora destroy){
    glist_destruir(pila->lista, destroy);
    free(pila);
}

int gpila_es_vacia(GPila pila){
    return(pila->lista == NULL);

}

void* pila_tope(GPila pila){
    return pila->lista->data;
}

void pila_apilar(GPila pila, void* dato, FuncionCopia copy){
    glist_agregar_inicio(&(pila->lista), dato, copy);
}


void pila_desapilar(GPila pila){
    GList temp = pila->lista;
    pila->lista = pila->lista->next;
    
    free(temp);

}

GList dar_vuelta(GList lista, FuncionCopia copy){
    GPila pila = crear_gpila();
    pila->lista = lista;

    GList nuevaLista = glist_crear();

    for(;(!gpila_es_vacia(pila));){
        glist_agregar_inicio(&nuevaLista, pila_tope(pila), copy);
        pila_desapilar(pila);

    }
    return nuevaLista;
}

void pila_imprimir(GPila pila, FuncionVisitante visit){
    glist_recorrer(pila->lista, visit);
}
