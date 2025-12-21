#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "slist.h"
#include "avl.h"
#include "HybridTable.h"

typedef void (*FuncionVisitante) (void* dato);
typedef void *(*FuncionCopiadora)(void *dato);
typedef int (*FuncionComparadora)(void *, void *);
typedef void (*FuncionDestructora)(void *dato);
typedef void (*FuncionVisitanteExtra)(void *dato, void *extra);
typedef unsigned (*FuncionHash)(void *dato);





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

void nodestroy(void* dato){
  return;
}



// HybridHash insertar
void tablahash_insertar(HybridHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Insertar el dato si la casilla estaba libre.
  if (tabla->elems[idx].head == NULL && tabla->elems[idx].modo == LIST)  
  { // Si la casilla no tenia un dato, el modo deberia estar en List, creamos una lista e insertamos al principio
    tabla->elems[idx].head = slist_agregar_inicio((SList)tabla->elems[idx].head, dato ,tabla->copia);
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
            SNodo* temp = (SList)tabla->elems[idx].head;
            AVL arbol = avl_crear();
            while(temp != NULL){
                avl_insertar(arbol, temp->dato, tabla->copia, tabla->comp, tabla->destr);
                temp = temp->sig;
            }
            slist_destruir(tabla->elems[idx].head, tabla->destr);
            tabla->elems[idx].head = arbol;
            tabla->elems[idx].modo = RAVL;
            avl_insertar((AVL)tabla->elems[idx].head,dato, tabla->copia, tabla->comp, tabla->destr);
            tabla->elems[idx].cantidad++;
        }
        return;
    }
    return;
  }
  // El modo del casillero esta en AVL
  else {
        if(avl_buscar((AVL)tabla->elems[idx].head, dato, tabla->comp)){
          avl_insertar((AVL)tabla->elems[idx].head, dato, tabla->copia, tabla->comp, tabla->destr);
        }
        else{
          avl_insertar((AVL)tabla->elems[idx].head, dato, tabla->copia, tabla->comp, tabla->destr);
          tabla->elems[idx].cantidad++;
        }
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
    return NULL;
}

void avl_to_lista(AVL_Nodo* arbol, FuncionCopiadora copy, SList* lista){
  if(arbol == NULL){
    return;
  }
  SNodo* nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = arbol->dato; // Asigno el mismo dato, despues para destruir el arbol no destruyo los mismos
  nuevoNodo->sig = *lista;
  *lista = nuevoNodo;
  avl_to_lista(arbol->izq, copy, lista);
  avl_to_lista(arbol->der, copy, lista);
}
void convertir_avl(AVL arbol, FuncionCopiadora copy, SList* lista){
  avl_to_lista(arbol->raiz, copy, lista);
}


// /**
//  * Elimina el dato de la tabla que coincida con el dato dado.
//  */
void tablahash_eliminar(HybridHash tabla, void *dato) {

  // 1. Calculamos la posición
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Si está vacío, chau
  if (tabla->elems[idx].head == NULL) return;

  // CASO LISTA: Borrado normal
  if (tabla->elems[idx].modo == LIST) {
      // Importante: verificar si realmente existe antes de restar cantidad
      // (Asumimos que slist_eliminar borra si existe)
      if (slist_buscar((SList)tabla->elems[idx].head, dato, tabla->comp)) {
          tabla->elems[idx].head = slist_eliminar((SList)tabla->elems[idx].head, dato, tabla->comp, tabla->destr);
          tabla->elems[idx].cantidad--;
      }
      return;
  }
  
  // CASO AVL: El arreglo crítico
  else if (tabla->elems[idx].modo == RAVL) {
      
      // 1. Verificamos si el dato existe en el AVL
      if (avl_buscar((AVL)tabla->elems[idx].head, dato, tabla->comp)) {
          
          // 2. Eliminamos del AVL
          avl_eliminar((AVL)tabla->elems[idx].head, dato, tabla->comp, tabla->copia, tabla->destr);
          
          // 3. ¡CORRECCIÓN! Restamos la CANTIDAD del bucket, NO la capacidad de la tabla
          tabla->elems[idx].cantidad--; 

          // 4. Chequeo del Umbral T/2 (Enunciado)
          // Si quedaron menos de 5 elementos (10 / 2), volvemos a Lista
          if (tabla->elems[idx].cantidad < (LIST2AVL / 2)) {
              
              // a. Creamos la lista nueva
              SList lista = slist_crear();
              
              // b. Volcamos los datos del AVL a la Lista
              convertir_avl((AVL)tabla->elems[idx].head, tabla->copia, &lista);
              
              // c. Destruimos el esqueleto del AVL (sin matar los datos que ya copiamos)
              avl_destruir((AVL)tabla->elems[idx].head, nodestroy);
              
              // d. Actualizamos el bucket
              tabla->elems[idx].head = lista;
              tabla->elems[idx].modo = LIST;
          }
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