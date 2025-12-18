#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "slist.h"
#include "avl.h"

typedef void (*FuncionVisitante) (void* dato);
typedef void *(*FuncionCopiadora)(void *dato);
typedef int (*FuncionComparadora)(void *, void *);
typedef void (*FuncionDestructora)(void *dato);
typedef void (*FuncionVisitanteExtra)(void *dato, void *extra);
typedef unsigned (*FuncionHash)(void *dato);



/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef enum{RAVL, LIST}
ModoCasilla;

typedef struct {
  void* head;        //Puede ser el puntero a una raiz de AVL o una lista enlazada
  int cantidad;      // Llevar la cuenta y saber cuando cambiar de lista a avl
  ModoCasilla modo; // El modo de la casilla
} CasillaHash;

/**
 * Estructura principal que representa la tabla hash.
 */
struct _TablaHash {
  CasillaHash *elems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};
typedef struct _TablaHash *HybridHash;

// #####################################################################################################################
#define TABLE_CAP 11 // La capacidad de la tabla esta predefinida
#define LIST2AVL 10

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
HybridHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash) {

  // Pedimos memoria para la estructura principal y las casillas.
  HybridHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(CasillaHash) * capacidad);
  assert(tabla->elems != NULL);

  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->destr = destr;
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx].head = NULL;
    tabla->elems[idx].modo = LIST; // Inician en modo lista
    tabla->elems[idx].cantidad = 0;
  }

  return tabla;
}


/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(HybridHash tabla) { return tabla->capacidad; }

/**
 * Destruye la tabla.
 */
// void tablahash_destruir(TablaHash tabla) {

//   // Destruir cada uno de los datos.
//   for (unsigned idx = 0; idx < tabla->capacidad; ++idx)
//     if (tabla->elems[idx].dato != NULL)
//       tabla->destr(tabla->elems[idx].dato);

//   // Liberar el arreglo de casillas y la tabla.
//   free(tabla->elems);
//   free(tabla);
//   return;
// }



// HybridHash insertar
void tablahash_insertar(HybridHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Insertar el dato si la casilla estaba libre.
  if (tabla->elems[idx].head == NULL && tabla->elems[idx].modo == LIST)  
  { // Si la casilla no tenia un dato, el modo deberia estar en List, creamos una lista e insertamos al principio
    SNodo* nuevonodo = malloc(sizeof(SNodo));
    nuevonodo->dato = tabla->copia(dato);
    nuevonodo->sig = (SNodo*)tabla->elems[idx].head;
    tabla->elems[idx].head = nuevonodo;
    tabla->elems[idx].cantidad++;
    return;
  }
  // Sobrescribir el dato si el mismo ya se encontraba en la tabla.
  else if (tabla->elems[idx].modo == LIST && tabla->elems[idx].head != NULL) 
  {
    SNodo* encontrado = slist_retornar((SList)tabla->elems[idx].head, dato, tabla->comp);
    if(encontrado){ // Sobreescribimos
        tabla->destr(encontrado->dato);
        encontrado->dato = tabla->copia(dato);
        return;
    }
    else
    { // El dato no se encontro en la lista, agregamos al principio siempre que se pueda
        if(tabla->elems[idx].cantidad < LIST2AVL){ // Insertamos al principio
            tabla->elems[idx].head = slist_agregar_inicio((SList)tabla->elems[idx].head, dato, tabla->copia);
            tabla->elems[idx].cantidad++;
        }
        else
        { // Debemos convertir la lista a AVL
            SNodo* temp = tabla->elems[idx].head;
            AVL arbol = avl_crear();
            while(temp != NULL){
                avl_insertar(arbol, temp->dato, tabla->copia, tabla->comp);
                temp = temp->sig;
            }
            slist_destruir(tabla->elems[idx].head, tabla->destr);
            tabla->elems[idx].head = arbol;
            tabla->elems[idx].modo = RAVL;
            avl_insertar((AVL)tabla->elems[idx].head,dato, tabla->copia, tabla->comp);
            tabla->elems[idx].cantidad++;
        }
    }
    return;
  }
  // El modo del casillero esta en AVL
  else {
    avl_insertar((AVL)tabla->elems[idx].head, dato, tabla->copia, tabla->comp);
    tabla->elems[idx].cantidad++;
    return;
  }
}

// /**
//  * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
//  * buscado no se encuentra en la tabla.
//  */
void* tablahash_buscar(HybridHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  if(tabla->elems[idx].modo == LIST){ // No veo si la cabeza es NULL, slist retornar dato se encarga de eso
    return slist_retornar_dato((SList)tabla->elems[idx].head, dato, tabla->comp);
    }
    
  if(tabla->elems[idx].modo == RAVL){
    // No chequeo si es NULL ya que si es AVL deberia tener mas o igual que TABLE_CAP (Ademas avl buscar retornar se encarga de eso)
    return avl_retornar((AVL)tabla->elems[idx].head, dato, tabla->comp);
    }
}

void avl_to_lista(AVL_Nodo* arbol, FuncionCopiadora copy, SList* lista){
  if(arbol == NULL){
    return;
  }
  SNodo* nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = arbol->dato; // Asigno el mismo dato, despues para destruir el arbol no destruyo los mismos
  nuevoNodo->sig = *lista;
  avl_to_lista(arbol->izq, copy, lista);
  avl_to_lista(arbol->der, copy, lista);
}
void convertir_avl(AVL arbol, FuncionCopiadora copy, SList* lista){
  avl_to_lista(arbol->raiz, copy, lista);
}

void nodestroy(void* dato){
  return;
}

// /**
//  * Elimina el dato de la tabla que coincida con el dato dado.
//  */
void tablahash_eliminar(HybridHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Retornar si la casilla estaba vacia.
  if (tabla->elems[idx].head == NULL)
    return;
  // Fijarse si el modo estaba en list, si es asi eliminar de la lista
  else if (tabla->elems[idx].modo == LIST) {
      
      tabla->elems[idx].head = slist_eliminar((SList)tabla->elems[idx].head, dato, tabla->comp, tabla->destr);
      tabla->elems[idx].cantidad--;
    return;
  }
  else if(tabla->elems[idx].modo == RAVL)
  {// Hay que contemplar el caso en el que se vuelva a Lista
    if(tabla->elems[idx].cantidad >= LIST2AVL){
      avl_eliminar((AVL)tabla->elems[idx].head, dato, tabla->comp, tabla->copia, tabla->destr);
      tabla->capacidad--;
    }
    else
    {// Tenemos que revertir el arbol a una lista
      SList lista = slist_crear();
      convertir_avl((AVL)tabla->elems[idx].head, tabla->copia, &lista);
      avl_destruir((AVL)tabla->elems[idx].head, nodestroy);
      tabla->elems[idx].modo = LIST;
      tabla->elems[idx].head = lista;
      tabla->elems[idx].head = slist_eliminar((SList)tabla->elems[idx].head, dato, tabla->comp, tabla->destr);
      tabla->elems[idx].cantidad--;
    }

  }
}

void tabla_destruir(HybridHash tabla){
  for(int i = 0; i<tabla->capacidad; i++){
    if(tabla->elems[i].modo == LIST && tabla->elems[i].head != NULL){
      slist_destruir((SList)tabla->elems[i].head, tabla->destr);
    }
    if(tabla->elems[i].modo == RAVL){
      avl_destruir((AVL)tabla->elems[i].head, tabla->destr);
    }
  }
  free(tabla->elems);
  free(tabla);
}