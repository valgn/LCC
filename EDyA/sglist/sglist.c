#include "sglist.h"
#include <stdio.h>
#include <stdlib.h>


SGList sglist_crear(){return NULL;}

void sglist_destruir(SGList lista, FuncionDestructora destroy){
    GNode* nodod;
    while (lista != NULL){
        nodod = lista;
        lista = lista->next;
        destroy(nodod->data);
        free(nodod);
    }
}

int sglist_vacia(SGList lista){
    return (lista == NULL);
}

void  sglist_recorrer(GList lista, FuncionVisitante f){
    for(;lista != NULL;lista = lista->next){
        f(lista->data);
    }
}

SGList sglist_insertar(SGList lista, void* dato, FuncionCopia c, FuncionComparadora comp){
    GNode* nuevo = malloc(sizeof(GNode));
    nuevo->data = c(dato);
    nuevo->next = NULL;

    
    if (lista == NULL || comp(dato, lista->data)) {
        nuevo->next = lista;
        return nuevo;
    }

    
    GNode* actual = lista;
    while (actual->next != NULL && !comp(dato, actual->next->data)) {
        actual = actual->next;
    }

    nuevo->next = actual->next;
    actual->next = nuevo;

    return lista;
}


int sglist_buscar(GList lista, void* dato, FuncionComparadora f){
    // GNode* temp = lista;

    while(lista != NULL && !f(lista->data, dato)){
        lista = lista->next;
    }

    if(lista == NULL){
        return 0;
    }
    else return 1;

}

SGList sglist_arr(void **arr, int len, FuncionCopia f, FuncionComparadora comp){
    SGList lista = sglist_crear();

    for(int i = 0; i < len; i++){
        lista = sglist_insertar(lista, arr[0], f, comp);
    }

    return lista;
}