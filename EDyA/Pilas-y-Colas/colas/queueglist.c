#include "queueglist.h"
#include <stdio.h>
#include <stdlib.h>


GList glist_crear(){return NULL;}

void glist_destruir(GList lista, FuncionDestructora destroy){
    GNode* nodod;
    while (lista != NULL){
        nodod = lista;
        lista = lista->next;
        destroy(nodod->data);
        free(nodod);
    }
}

int glist_vacia(GList lista){
    return (lista == NULL);
}

void  glist_recorrer(GList lista, FuncionVisitante f){
    for(;lista != NULL;lista = lista->next){
        f(lista->data);
    }
}

GList glist_insertar(GList lista, void* dato, FuncionCopia c, FuncionComparadora comp){
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


int glist_buscar(GList lista, void* dato, FuncionComparadora f){
    // GNode* temp = lista;

    while(lista != NULL && !f(lista->data, dato)){
        lista = lista->next;
    }

    if(lista == NULL){
        return 0;
    }
    else return 1;

}

GList glist_arr(void **arr, int len, FuncionCopia f, FuncionComparadora comp){
    GList lista = glist_crear();

    for(int i = 0; i < len; i++){
        lista = glist_insertar(lista, arr[0], f, comp);
    }

    return lista;
}