#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h>

typedef void (*FuncionVisitante) (void* dato);
typedef void *(*FuncionCopiadora)(void *dato);
typedef void (*FuncionDestructora)(void *dato);
typedef int (*FuncionComparadora)(void *, void *);

typedef struct _SNodo {
  void* dato;
  struct _SNodo *sig;
} SNodo;

typedef SNodo* SList;

/**
 * Devuelve una lista vacía.
 */
SList slist_crear();

/**
 * Destruccion de la lista.
 */
void slist_destruir(SList lista, FuncionDestructora destroy);

/**
 * Determina si la lista es vacía.
 */
int slist_vacia(SList lista);

/**
 * Agrega un elemento al final de la lista.
 */
SList slist_agregar_final(SList lista, void* dato, FuncionCopiadora copy);

/**
 * Agrega un elemento al inicio de la lista.
 */
SList slist_agregar_inicio(SList lista, void* dato, FuncionCopiadora copy);

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void slist_recorrer(SList lista, FuncionVisitante visit);

int slist_buscar(SList lista, void* dato, FuncionComparadora comp);

SNodo* slist_retornar(SList lista, void* dato, FuncionComparadora comp);

void* slist_retornar_dato(SList lista, void* dato, FuncionComparadora comp);

SList slist_eliminar(SList lista, void* dato, FuncionComparadora comp, FuncionDestructora destroy);

#endif /* __SLIST_H__ */
