#ifndef __DLIST_H__
#define __DLIST_H__

#include <stddef.h>

typedef void (*FuncionVisitante) (int a);

typedef struct _DNodo {
    int dato;
    struct _DNodo* sig;
    struct _DNodo* ant;
} DNodo;

typedef struct {
    DNodo* primero;
    DNodo* ultimo;
} DList;

typedef enum {
    DLIST_RECORRIDO_HACIA_ADELANTE,
    DLIST_RECORRIDO_HACIA_ATRAS
} DListOrdenDeRecorrido;

DList* crearlista();

DList* meterinicio(DList* lista, int dato);

DList* meterultimo(DList* lista, int dato);


void recorrer_lista_adelante();

void recorrer_lista_atras();

int dlistlen(DList* lista);



#endif