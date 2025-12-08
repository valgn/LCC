#include "bstree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Estructura del nodo del arbol de busqueda binaria.
 * Tiene un puntero al dato (dato),
 * un puntero al nodo raiz del subarbol izquierdo (izq),
 * y un puntero al nodo raiz del subarbol derecho (der).
 */


BSTree crear_nodo(void *dato, FuncionCopiadora copy) {
    BSTree nodo = malloc(sizeof(struct _BST_Nodo));
    nodo->dato = copy(dato);
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

/**
 * bstee_crear: Retorna un arbol de busqueda binaria vacio
 */
BSTree bstee_crear() { return NULL; }

/**
 * bstree_destruir: Destruye el arbol y sus datos
 */
void bstree_destruir(BSTree raiz, FuncionDestructora destr) {
  if (raiz != NULL) {
    bstree_destruir(raiz->izq, destr);
    bstree_destruir(raiz->der, destr);
    destr(raiz->dato);
    free(raiz);
  }
};

/**
 * bstree_buscar: Retorna 1 si el dato se encuentra y 0 en caso
 * contrario
 */
int bstree_buscar(BSTree raiz, void *dato, FuncionComparadora comp) {
  if (raiz == NULL)
    return 0;
  else if (comp(dato, raiz->dato) == 0) // raiz->dato == dato
    return 1;
  else if (comp(dato, raiz->dato) < 0) // dato < raiz->dato
    return bstree_buscar(raiz->izq, dato, comp);
  else // raiz->dato < dato
    return bstree_buscar(raiz->der, dato, comp);
}

/**
 * bstree_insertar: Inserta un dato no repetido en el arbol, manteniendo la
 * propiedad del arbol de busqueda binaria
 */
BSTree bstree_insertar(BSTree raiz, void *dato, FuncionCopiadora copia,
                       FuncionComparadora comp) {
  if (raiz == NULL) { // insertar el dato en un nuevo nodo
    struct _BST_Nodo *nuevoNodo = malloc(sizeof(struct _BST_Nodo));
    assert(nuevoNodo != NULL);

    nuevoNodo->dato = copia(dato);
    nuevoNodo->izq = nuevoNodo->der = NULL;
    return nuevoNodo;
  } else if (comp(dato, raiz->dato) < 0) // dato < raiz->dato
    raiz->izq = bstree_insertar(raiz->izq, dato, copia, comp);
  else if (comp(dato, raiz->dato) > 0) // raiz->dato < dato
    raiz->der = bstree_insertar(raiz->der, dato, copia, comp);
  // si el dato ya se encontraba, no es insertado
  return raiz;
}

/**
 * bstree_recorrer: Recorrido DSF del arbol
 */
void bstree_recorrer(BSTree raiz, BSTreeRecorrido orden,
                     FuncionVisitanteExtra visita, void *extra) {
  if (raiz != NULL) {
    if (orden == BTREE_RECORRIDO_PRE)
      visita(raiz->dato, extra);
    bstree_recorrer(raiz->izq, orden, visita, extra);
    if (orden == BTREE_RECORRIDO_IN)
      visita(raiz->dato, extra);
    bstree_recorrer(raiz->der, orden, visita, extra);
    if (orden == BTREE_RECORRIDO_POST)
      visita(raiz->dato, extra);
  }
}

BSTree bstree_eliminar(BSTree arbol, void *dato, FuncionComparadora comp, FuncionDestructora destroy, FuncionCopiadora copy){
  if(arbol == NULL){
    return NULL;
  }
  int comparacion = comp(arbol->dato, dato);
  if(comparacion == 0){
        if (!arbol->izq && !arbol->der) {
            destroy(arbol->dato);
            free(arbol);
            return NULL;
        }

    else if((!arbol->der || !arbol->izq) && !(!arbol->der && !arbol->izq)){
      BSTree hijo;
      destroy(arbol->dato);
      if(!arbol->der){
        hijo = arbol->izq;
      }
      else hijo = arbol->der;
      free(arbol);
      return hijo;
    }

    else{
      BSTree sucesor = arbol->der;
      while (sucesor->izq != NULL){
        sucesor = sucesor->izq;
      }

      destroy(arbol->dato);
      arbol->dato = copy(sucesor->dato);

      arbol->der = bstree_eliminar(arbol->der, sucesor->dato, comp, destroy, copy);
    }
  }
    else if (comparacion < 0) {
        arbol->der = bstree_eliminar(arbol->der, dato, comp, destroy, copy);
    } 
    else {
        arbol->izq = bstree_eliminar(arbol->izq, dato, comp, destroy, copy);
    }

    return arbol;
}


void* bstree_k_esimo_menor_aux(BSTree arbol, int* k){
  if(arbol== NULL){
    return NULL;
  }
  void* izq = bstree_k_esimo_menor_aux(arbol->izq, k);
  if(izq != NULL) return izq;
  (*k)--;
  if(*k == 0) return arbol->dato;
  return bstree_k_esimo_menor_aux(arbol->der, k);

}

void* bstree_k_esimo_menor(BSTree arbol, int k){
  return bstree_k_esimo_menor_aux(arbol, &k);
}

int btree_validar(BSTree arbol, FuncionComparadora comp, void* max, void* min){
  if (arbol == NULL) return 1;
  if ((max != NULL && (comp(arbol->dato, max) >= 0)) || (min != NULL && (comp(arbol->dato, min) <= 0))) return 0;
  return btree_validar(arbol->izq, comp, arbol->dato, min) && btree_validar(arbol->der, comp, max, arbol->dato);
}