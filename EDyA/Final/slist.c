#include "slist.h"
#include <stdlib.h>
#include <stdio.h>
SList slist_crear() {
  return NULL;
}


void slist_destruir(SList lista, FuncionDestructora destroy) {
  SNodo *nodoAEliminar;
  while (lista != NULL) {
    nodoAEliminar = lista;
    lista = lista->sig;
    destroy(nodoAEliminar->dato);
    free(nodoAEliminar);
  }
}

int slist_vacia(SList lista) {
  return lista == NULL;
}

SList slist_agregar_final(SList lista, void* dato, FuncionCopiadora copy) {
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = NULL;

  if (lista == NULL)
    return nuevoNodo;

  SList nodo = lista;
  for (;nodo->sig != NULL;nodo = nodo->sig);
  /* ahora 'nodo' apunta al ultimo elemento en la lista */

  nodo->sig = copy(nuevoNodo);
  return lista;
}

SList slist_agregar_inicio(SList lista, void* dato, FuncionCopiadora copy) {
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = copy(dato);
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

void slist_recorrer(SList lista, FuncionVisitante visit) {
  for (SNodo *nodo = lista; nodo != NULL; nodo = nodo->sig)
    visit(nodo->dato);
}

int slist_buscar(SList lista, void* dato, FuncionComparadora comp){
  SNodo* temp = lista; 
  while(temp != NULL && comp(temp->dato, dato) != 0) temp = temp->sig;
  if(temp == NULL) return 0;
  return 1;
}

SNodo* slist_retornar(SList lista, void* dato, FuncionComparadora comp){
  SNodo* temp = lista; 
  while(temp != NULL && comp(temp->dato, dato) != 0) temp = temp->sig;
  if(temp == NULL) return NULL;
  return temp;
}

void* slist_retornar_dato(SList lista, void* dato, FuncionComparadora comp){
  SNodo* temp = lista; 
  while(temp != NULL && comp(temp->dato, dato) != 0) temp = temp->sig;
  if(temp == NULL) return NULL;
  return temp->dato;
}

SList slist_eliminar(SList lista, void* dato, FuncionComparadora comp, FuncionDestructora destroy){
  SNodo* temp = lista;
  SNodo* ant = NULL;
  while(temp != NULL && comp(temp->dato, dato) != 0){
    ant = temp;
    temp = temp->sig;
  }
  if(temp == NULL) return NULL;
  ant->sig = temp->sig;
  destroy(temp->dato);
  free(temp);
  return lista;
}

