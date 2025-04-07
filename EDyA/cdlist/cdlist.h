#ifndef __DLIST_H__
#define __DLIST_H__

#include <stddef.h>

typedef void (*FuncionVisitante) (int a);


typedef struct _CDNodo {
    int dato;
    struct _CDNodo* sig;
    struct _CDNodo* ant;
} CDNodo;

typedef struct {
    CDNodo* primero;

} CDList;

typedef enum {
    DLIST_RECORRIDO_HACIA_ADELANTE,
    DLIST_RECORRIDO_HACIA_ATRAS
} DListOrdenDeRecorrido;

CDList* crear();

void agregar(CDList* lista, int dato);

void recorrer(CDList* lista, FuncionVisitante f);

void recorrer2(CDList* lista, FuncionVisitante f);

// DList* meterultimo(DList* lista, int dato);


// void recorrer_lista_adelante();

// void recorrer_lista_atras();





#endif