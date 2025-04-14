#ifndef ___GPILAS___
#define ___GPILAS___

#include "glist.h"

struct _GPilas{
    GList lista;

};

typedef struct _GPilas *GPila;

GPila crear_gpila();

void gpila_es_vacia(GPila pila);

void gpila_destruir(GPila pila, FuncionDestructora destroy);

int pila_tope(GPila pila);

void pila_apilar(GPila pila, int dato, FuncionCopia copy);

void pila_desapilar(GPila pila);

void pila_imprimir(GPila pila, FuncionVisitante visit);


#endif