#ifndef ___QUEUE_H___
#define ___QUEUE_H___

#include "queueglist.h"

typedef void*(*FuncionReturn)(void* dato);



typedef struct _ColaRep {
  GNode *frente;
  GNode *ultimo;
} *Cola;

Cola cola_crear();

void cola_destruir(Cola cola, FuncionDestructora destroy);

int cola_es_vacia(Cola cola);

void* cola_inicio(Cola cola);

void encolar(Cola cola, void* dato, FuncionCopia copy);

void* desencolar(Cola cola);

void cola_imprimir(Cola cola, FuncionVisitante func);
#endif