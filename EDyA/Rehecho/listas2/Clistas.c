#include <stdio.h>
#include <stdlib.h>


typedef void (*FuncionDestructora)(void*);
typedef int (*FuncionPredicado)(void*);
typedef void* (*FuncionCopiadora)(void*);
typedef void (*FuncionVisitante)(void*);
typedef int (*FuncionComparadora)(void*, void*);

/*
Funciones para main
*/
void destruir_entero(void* dato){
    free(dato);
}
void* copiar_entero(void* dato){
    int* val = malloc(sizeof(int));
    assert(val);
    *val = *(int*)dato;
    return val;
}

void imprimir_entero(void* dato){
    printf("%d ", *(int*)dato);
}
int comparar_entero(void* dato1, void* dato2){
    return *(int*)dato1 - *(int*)dato2;
}
int es_par(void* dato){
    return !(*(int*)dato % 2);
}
// ---------------------------

typedef struct _Nodo{
    void* dato;
    struct _Nodo* sig, *ant;
}Nodo;

typedef Nodo* CList;

CList crear_circular(){
    return NULL;
}
void destruir_circular(CList lista, FuncionDestructora destroy){
    Nodo* actual = lista;
    if(lista == NULL) return;
    actual->ant->sig = NULL;

    while(actual != NULL){
        Nodo* temp = actual;
        actual = actual->sig;
        destroy(temp->dato);
        free(temp);
    }
    return;
}

void insertar_circular_adelante(CList* lista, void* dato, FuncionCopiadora copy){
    Nodo* nuevo = malloc(sizeof(Nodo));
    nuevo->dato = copy(dato);
    if((*lista) == NULL){
        nuevo->sig = nuevo;
        nuevo->ant = nuevo;
        return;
    }
    else{
        (*lista)->ant->sig = nuevo;
        nuevo->sig = lista;

        nuevo->ant = (*lista)->ant;
        (*lista)->ant = nuevo;
        return;
    }
}
