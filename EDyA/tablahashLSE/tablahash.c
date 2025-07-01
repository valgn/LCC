#include "tablahash.h"
#include <assert.h>
#include <stdlib.h>

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */

typedef struct _NodoHash{
  void* dato;
  struct _NodoHash* sig;
}NodoHash;

typedef struct {
  NodoHash* cabezon;
} CasillaHash;

/**
 * Estructura principal que representa la tabla hash.
 */
struct _TablaHash {
  CasillaHash *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

NodoHash* crear_nodo(void* dato, FuncionCopiadora copia){
  NodoHash* nuevoNodo = malloc(sizeof(struct _NodoHash));
  nuevoNodo->dato = copia(dato);
  nuevoNodo->sig = NULL;
  return nuevoNodo;
}

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash) {

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(CasillaHash) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->destr = destr;
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx].cabezon = NULL;
  }

  return tabla;
}

/**
 * Retorna el numero de elementos de la tabla.
 */
int tablahash_nelems(TablaHash tabla) { return tabla->numElems; }

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla) { return tabla->capacidad; }

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla) {

  // Destruir cada uno de los datos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx){
    NodoHash* nodo = tabla->elems[idx].cabezon;
    while(nodo != NULL){
      NodoHash* sig = nodo->sig;
      tabla->destr(nodo->dato);
      free(nodo);
      nodo = sig;
    }
  }

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */
void tablahash_insertar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  NodoHash* nodo = tabla->elems[idx].cabezon;
  // Insertar el dato si la casilla estaba libre.

  while(nodo != NULL){
    if(tabla->comp(nodo->dato, dato) == 0){
      tabla->destr(nodo->dato);
      nodo->dato = tabla->copia(dato);
      return;
    }
    nodo = nodo->sig;
  }
  NodoHash* nuevo = malloc(sizeof(struct _NodoHash));
  nuevo->dato = tabla->copia(dato);
  nuevo->sig = tabla->elems[idx].cabezon;
  tabla->elems[idx].cabezon = nuevo;
  tabla->numElems++;
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  NodoHash* nodo = tabla->elems[idx].cabezon;
  while(nodo != NULL){
    if(tabla->comp(nodo->dato, dato) == 0){
      return nodo->dato;
    }
    nodo = nodo->sig;
  }
  return NULL;
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  NodoHash* nodo = tabla->elems[idx].cabezon;
  NodoHash* ant = NULL;

  while(nodo != NULL){
    if(tabla->comp(nodo->dato, dato) == 0){
      if(ant == NULL){
        tabla->elems[idx].cabezon = nodo->sig;
      }
      else{
        ant->sig = nodo->sig;
      }
      tabla->destr(nodo->dato);
      free(nodo);
      tabla->numElems--;
      return;


    }
    ant = nodo;
    nodo = nodo->sig;
  }
}
