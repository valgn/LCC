#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include <assert.h>
#include <stdlib.h>


SList slist_crear() {
  return NULL;
}

void slist_destruir(SList lista) {
  SNodo *nodoAEliminar;
  while (lista != NULL) {
    nodoAEliminar = lista;
    lista = lista->sig;
    free(nodoAEliminar);
  }
}

int slist_vacia(SList lista) {
  return lista == NULL;
}

SList slist_agregar_inicio(SList lista, int dato) {
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

void slist_recorrer(SList lista, FuncionVisitante visit) {
  for (SNodo *nodo = lista; nodo != NULL; nodo = nodo->sig)
    visit(nodo->dato);
}

int slist_buscar(SList lista, int dato){
    SNodo* temp = lista;
    while(temp != NULL && temp->dato != dato){
        temp = temp->sig;
    }
    if(temp == NULL){
        return 0;
    }
    else return 1;
}


TablaHash tablahash_crear(unsigned capacidad, FuncionHasheadora hash) {

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(CasillaHash) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx].lista = slist_crear();
  }

  return tabla;
}

int tablahash_nelems(TablaHash tabla) { return tabla->numElems; }

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla) { return tabla->capacidad; }

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla) {
    for(int i = 0; i < tablahash_capacidad(tabla); i++){
        if(!slist_vacia(tabla->elems[i].lista)) slist_destruir(tabla->elems[i].lista); 
    }
    free(tabla->elems);
    free(tabla);
}



void tablahash_insertar(TablaHash tabla, int i, int j) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(i) % tabla->capacidad;

  // Insertar el dato si la casilla estaba libre.
  if ((slist_vacia(tabla->elems[idx].lista))) {
    tabla->numElems++;
     tabla->elems[idx].lista = slist_agregar_inicio(tabla->elems[idx].lista, j);
    return;
  }
  // Sobrescribir el dato si el mismo ya se encontraba en la tabla.
  else if (slist_buscar(tabla->elems[idx].lista, j)){
    return;
  }
  // No hacer nada si hay colision.
  else {
    tabla->elems[idx].lista = slist_agregar_inicio(tabla->elems[idx].lista, j);
    return;
  }
}

SList slist_eliminar(SList lista, int dato){
    if(slist_vacia(lista))
    return lista;

    if(lista->dato == dato){
        SNodo* temp = lista;
        lista = lista->sig;
        free(temp);
        return lista;
    }

    else{
        SNodo* temp = lista;
        while(temp->sig != NULL && temp->sig->dato != dato){
            temp = temp->sig;
        }
        if(temp->sig == NULL) return lista;
        else{
            SNodo* borrar = temp->sig;
            temp->sig = temp->sig->sig;
            free(borrar);
            return lista;

        }
    }
}

void tablahash_borrar(TablaHash tabla, int i, int j){
    if(i > tabla->capacidad || j > tabla->capacidad){
        perror("Indices incorrectos.");
        return;
    }
    
    int idx = tabla->hash(i) % tabla->capacidad;
    if(slist_vacia(tabla->elems[idx].lista)){
        return;
    }
    else{
        tabla->elems[idx].lista = slist_eliminar(tabla->elems[idx].lista, j);
    }
}

int es_simetrica(TablaHash tabla){
    if(tabla->numElems == 0) return 1;
    int i = 0;
    int f = 1;
    while(i < tabla->capacidad && f){
        int idx = tabla->hash(i) % tabla->capacidad;
        SNodo* actual = tabla->elems[idx].lista;
        while(!slist_vacia(actual)){
            int j = actual->dato;
            if(i!=j && !(slist_buscar(tabla->elems[tabla->hash(j) % tabla->capacidad].lista, i))) f = 0;
            actual = actual->sig;
        }
        i++;
    }

    return f;
}
