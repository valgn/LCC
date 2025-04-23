#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

Cola cola_crear() {
    Cola cola = malloc(sizeof(Cola));
    cola->frente = cola->ultimo = NULL;
    return cola;
}

void cola_destruir(Cola cola, FuncionDestructora destroy) {
    // Destruir todos los nodos (similar a glist_destruir pero empezando desde cola->frente)
    GNode *nodo = cola->frente;
    while (nodo != NULL) {
        GNode *temp = nodo;
        nodo = nodo->next;
        if (destroy != NULL) {
            destroy(temp->data);  // Liberar el dato del nodo
        }
        free(temp);  // Liberar el nodo
    }
    // Liberar la estructura de la cola
    free(cola);
}

int cola_es_vacia(Cola cola){
    return (cola->frente == NULL);
}

void* cola_inicio(Cola cola){
    return cola->frente->data;
}

void encolar(Cola cola, void* dato, FuncionCopia copy) {
    GNode* nuevo = malloc(sizeof(GNode));
    nuevo->data = copy(dato);
    nuevo->next = NULL;

    if (cola_es_vacia(cola)) {
        cola->frente = cola->ultimo = nuevo;
    } else {
        cola->ultimo->next = nuevo;
        cola->ultimo = nuevo;
    }
}

void* desencolar(Cola cola){
    GNode* temp = cola->frente;
    cola->frente = cola->frente->next;
    void* dato = temp->data;
    free(temp);
    return dato;
}

void cola_imprimir(Cola cola, FuncionVisitante func){
    GNode* temp = cola->frente;
    while(temp != NULL){
        func(temp->data);
        temp = temp->next;
    }
}