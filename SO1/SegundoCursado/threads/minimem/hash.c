#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include <string.h>
#include <pthread.h>

NodoHash* crear_nodo(char* dato){
  NodoHash* nuevoNodo = malloc(sizeof(struct _NodoHash));
  nuevoNodo->dato = malloc(sizeof(strlen(dato)) + 1);
  nuevoNodo->dato = strcpy(nuevoNodo->dato, dato);
  nuevoNodo->sig = NULL;
  return nuevoNodo;
}

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionHash hash) {

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
    tabla->elems[idx].head = NULL;
    pthread_mutex_init(&tabla->elems[idx].head->lock, NULL);
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

  
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx){
    NodoHash* nodo = tabla->elems[idx].head;
    while(nodo != NULL){
      NodoHash* sig = nodo->sig;
      free(nodo->dato);
      free(nodo);
      nodo = sig;
    }
  }

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}


// Corregir el tipo de retorno a float
float factor_carga(TablaHash hash){
  return (float)hash->numElems / hash->capacidad;
}

void rehashear(TablaHash vieja){
  int viejacap = vieja->capacidad;
  int nuevacap = viejacap * 2; // O el siguiente primo si te pones exquisito
  
  // 1. Crear el nuevo array de casillas
  CasillaHash* nuevoarr = malloc(sizeof(CasillaHash) * nuevacap);
  for(int i = 0; i < nuevacap; i++){
    nuevoarr[i].head = NULL;
  }

  // 2. Mover los nodos (Re-link)
  for(int i = 0; i < viejacap; i++){
    NodoHash* nodo = vieja->elems[i].head;
    while(nodo != NULL){
      NodoHash* next = nodo->sig; // Guardamos referencia al siguiente
      
      // Recalculamos posición
      unsigned newidx = vieja->hash(nodo->dato) % nuevacap;
      
      // Insertamos al principio de la nueva lista (O(1))
      nodo->sig = nuevoarr[newidx].head;
      nuevoarr[newidx].head = nodo; // <--- ¡LA LÍNEA QUE FALTABA!
      
      nodo = next;
    }
  } 

  // 3. Liberar el array viejo y actualizar la estructura
  free(vieja->elems);
  vieja->capacidad = nuevacap;
  vieja->elems = nuevoarr;
}

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */
void tablahash_insertar(TablaHash tabla, char* dato, unsigned int key) {
  if(factor_carga(tabla) > 0.7){
    rehashear(tabla);
  }
  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = key % tabla->capacidad;
  NodoHash* nodo = tabla->elems[idx].head;
  // Insertar el dato si la casilla estaba libre.

  while(nodo != NULL){
    if(!strcmp(nodo->dato, dato)){
      return;
    }
    nodo = nodo->sig;
  }
  NodoHash* nuevo = malloc(sizeof(struct _NodoHash));
  pthread_mutex_init(&nodo->lock, NULL);
  nodo->dato = malloc(sizeof(strlen(dato)) + 1);
  nodo->dato = strcpy(nodo->dato, dato);
  nuevo->sig = tabla->elems[idx].head;
  tabla->elems[idx].head = nuevo;
  tabla->numElems++;
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
char* tablahash_buscar(TablaHash tabla, char* dato, unsigned int key) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = key % tabla->capacidad;
  NodoHash* nodo = tabla->elems[idx].head;
  while(nodo != NULL){
    
    if(!strcmp(nodo->dato, dato)){
      return nodo->dato;
    }
    nodo = nodo->sig;
  }
  return NULL;
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, char* dato, unsigned int key) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = key % tabla->capacidad;
  NodoHash* nodo = tabla->elems[idx].head;
  NodoHash* ant = NULL;

  while(nodo != NULL){
    if(!strcmp(nodo->dato, dato)){
      if(ant == NULL){
        tabla->elems[idx].head = nodo->sig;
      }
      else{
        ant->sig = nodo->sig;
      }
      free(nodo->dato);
      free(nodo);
      tabla->numElems--;
      return;


    }
    ant = nodo;
    nodo = nodo->sig;
  }
}