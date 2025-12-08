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


void* pila_desapilar(GPila pila){ 
    if (gpila_es_vacia(pila)) return NULL;

    GList temp = pila->lista;
    void* dato = temp->data; // Rescatamos el dato
    
    pila->lista = pila->lista->next; // Desenganchamos
    
    free(temp); // Solo liberamos el nodo (GNode)
    
    return dato; // Devolvemos el dato VIVO al usuario
}

GList dar_vuelta(GList lista, FuncionCopia copy, FuncionDestructora destroy) {
    // 1. Creamos la nueva lista vacía
    GList nueva = glist_crear();

    // 2. Recorremos la original hacia adelante
    for (GNode* temp = lista; temp != NULL; temp = temp->next) {
        // Al insertar siempre al INICIO, invertimos el orden naturalmente
        glist_agregar_inicio(&nueva, temp->data, copy);
    }

    // 3. Destruimos la original (porque tu main espera que esta función consuma la lista vieja)
    glist_destruir(lista, destroy);

    return nueva;
}

void pila_imprimir(GPila pila, FuncionVisitante visit){
    glist_recorrer(pila->lista, visit);
}
