#ifndef ___PILAS___
#define ___PILAS___

#include "arregloenteros.h"

struct _Pila {
    ArregloEnteros *arr;
    int ultimo;
};
typedef struct _Pila *Pila;

Pila pila_crear(int capacidad);

void pila_destruir(Pila pila);

int pila_es_vacia(Pila pila);

int pila_tope(Pila pila);

void pila_apilar(Pila pila, int elemento);

void pila_desapilar(Pila pila);

void pila_imprimir(Pila pila);

#endif