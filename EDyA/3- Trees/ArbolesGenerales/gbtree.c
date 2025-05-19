#include "gbtree.h"
#include <assert.h>
#include <stdlib.h>

typedef struct _BTNodo {
  void* dato;
  struct _BTNodo *left;
  struct _BTNodo *right;
}BTNodo;


BTree btree_crear() { return NULL; }


void btree_destruir(BTree nodo, FuncionDestructora destroy) {
  if (nodo != NULL) {
    
    btree_destruir(nodo->left, destroy);
    btree_destruir(nodo->right, destroy);
    destroy(nodo->dato);
    free(nodo);
  }
}

int btree_empty(BTree nodo) { return nodo == NULL; }

BTree btree_unir(void* dato, BTree left, BTree right, FuncionCopia copy) {
  BTree nuevoNodo = malloc(sizeof(struct _BTNodo));
  assert(nuevoNodo != NULL);
  nuevoNodo->dato = copy(dato);
  nuevoNodo->left = left;
  nuevoNodo->right = right;
  return nuevoNodo;
}

void btree_recorrer(BTree arbol, BTreeOrdenDeRecorrido orden,
                    FuncionVisitante visit) {
  // Procesar la raiz, el izq y el der
  if(orden == BTREE_RECORRIDO_PRE){ 
    if(arbol == NULL){
      return;
    }
    visit(arbol->dato);
    btree_recorrer(arbol->left, orden, visit);
    btree_recorrer(arbol->right, orden, visit);
  }
  // Procesar el izq, la raiz, el der
  if(orden == BTREE_RECORRIDO_IN){ 
    if(arbol == NULL){
      return;
  }
    btree_recorrer(arbol->left, orden, visit);
    visit(arbol->dato);
    btree_recorrer(arbol->right, orden, visit);
  }
  // Procesar el izq, el der, la raiz
  if(orden == BTREE_RECORRIDO_POST){ 
      if(arbol == NULL){
      return;
  }
    btree_recorrer(arbol->left, orden, visit);
    btree_recorrer(arbol->right, orden, visit);
    visit(arbol->dato);
  }
  //assert(0);
  return;
}
