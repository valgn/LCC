#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"

DList* crearlista(){
    DList* lista = malloc(sizeof(DList));
    lista->primero = NULL;
    lista->ultimo = NULL;
    return lista;
}

DList* meterinicio(DList* lista, int dato){
    DNodo* nuevonodo = malloc(sizeof(DNodo));
    nuevonodo->dato = dato;
    nuevonodo->sig = lista->primero;
    nuevonodo->ant = NULL;

    if(lista->primero != NULL){
        lista->primero->ant = nuevonodo;

    }
    lista->primero = nuevonodo;
    
    if(lista->ultimo == NULL){
        lista->ultimo =  nuevonodo;
    }


    return lista;
}

DList* meterultimo(DList* lista, int dato){
    DNodo* nuevonodo = malloc(sizeof(DNodo));
    nuevonodo->sig = NULL;
    nuevonodo->ant = lista->ultimo;
    lista->ultimo = nuevonodo;
    nuevonodo->dato = dato;
    return lista;
}

void recorrer_lista_adelante(DList* lista){
    for (;lista->primero!= NULL;lista->primero=lista->primero->sig){
        printf("%d ", lista->primero->dato);

    }
    printf("\n");
}
void recorrer_lista_atras(DList* lista){
    for (;lista->ultimo!= NULL;lista->ultimo=lista->ultimo->ant){
        printf("%d ", lista->ultimo->dato);

    }
    printf("\n");
}

int dlistlen(DList* lista){
    int len = 0;
    for(;lista->primero->sig == NULL; lista->primero = lista->primero->sig, len++);


    return len;
}

void recorrer_lista(DList*  lista, DListOrdenDeRecorrido dir){
    if(dir == 0){
        recorrer_lista_adelante(lista);
    }
    else if(dir == 1){
        recorrer_lista_atras(lista);
    }
}