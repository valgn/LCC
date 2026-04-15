#ifndef ___HASH___H___
#define ___HASH___H___

#include <pthread.h>

typedef unsigned (*FuncionHash)(void *dato);

typedef struct _NodoHash{
    pthread_mutex_t lock;
    char* dato;
    struct _NodoHash* sig;
}NodoHash;

typedef struct {
  NodoHash* head;
} CasillaHash;


struct _TablaHash {
    CasillaHash *elems;
    unsigned numElems;
    unsigned capacidad;
    FuncionHash hash;
};

typedef struct _TablaHash *TablaHash;

#endif