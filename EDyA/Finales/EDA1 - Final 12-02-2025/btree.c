#include "btree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

struct _BTNodo {
  int dato;
  struct _BTNodo *left;
  struct _BTNodo *right;
};

/**
 * Devuelve un arbol vacío.
 */
BTree btree_crear() { return NULL; }

/**
 * Destruccion del árbol.
 */
void btree_destruir(BTree nodo) {
  if (nodo != NULL) {   
    btree_destruir(nodo->left);
    btree_destruir(nodo->right);
    free(nodo);
  }
}

/**
 * Indica si el árbol es vacío.
 */
int btree_empty(BTree nodo) { return nodo == NULL; }

/**
 * Crea un nuevo arbol, con el dato dado en el nodo raiz, y los subarboles dados
 * a izquierda y derecha.
 */
BTree btree_unir(int dato, BTree left, BTree right) {
  BTree nuevoNodo = malloc(sizeof(struct _BTNodo));
  assert(nuevoNodo != NULL);
  nuevoNodo->dato = dato;
  nuevoNodo->left = left;
  nuevoNodo->right = right;
  return nuevoNodo;
}

/**
 * Recorrido del arbol, utilizando la funcion pasada.
 */
void btree_recorrer(BTree arbol, BTreeOrdenDeRecorrido orden,
                    FuncionVisitante visit) {
  if(arbol == NULL){
    return;
  }
  switch (orden)
  {
  case BTREE_RECORRIDO_IN:
    btree_recorrer(arbol->left, orden, visit);
    visit(arbol->dato);
    btree_recorrer(arbol->right, orden, visit);
    break;
    
  case BTREE_RECORRIDO_POST:
    btree_recorrer(arbol->left, orden, visit);
    btree_recorrer(arbol->right, orden, visit);
    visit(arbol->dato);
    break;

  case BTREE_RECORRIDO_PRE:
    visit(arbol->dato);
    btree_recorrer(arbol->left, orden, visit);
    btree_recorrer(arbol->right, orden, visit);
  default:
    break;
  }  
}


static void btree_imprimir_helper(BTree arbol, int altura){
  if(arbol == NULL)
    return;
  if(arbol->right != NULL)
    btree_imprimir_helper(arbol->right, altura + 1);
  else{
    for(int i = 0; i != altura; i++)
      printf(" ");
    printf("%d\n", arbol->dato);
  }
  btree_imprimir_helper(arbol->left, altura + 1);
}

void btree_imprimir(BTree arbol){
  btree_imprimir_helper(arbol, 0);
}

BTree btree_insertar_random(int dato, BTree arbol){
  if(arbol == NULL){
    BTree nuevoNodo = malloc(sizeof(struct _BTNodo));
    nuevoNodo->dato = dato;
    nuevoNodo->left = NULL;
    nuevoNodo->right = NULL;
    return nuevoNodo; 
  }
  BTree *siguiente = (rand() % 2) ? &arbol->right : &arbol->left;
  *siguiente = btree_insertar_random(dato, *siguiente);

  return arbol;
}

BTree ABB_insertar(int dato, BTree arbol){
  if (arbol == NULL){
    BTree nuevoNodo = malloc(sizeof(struct _BTNodo));
    nuevoNodo->dato = dato;
    nuevoNodo->left = NULL;
    nuevoNodo->right = NULL;
    return nuevoNodo; 
  }
  else if(arbol->dato < dato)
    arbol->right = ABB_insertar(dato, arbol->right);
  else if(dato < arbol->dato)
    arbol->left = ABB_insertar(dato, arbol->left);
  //No insertamos si esta repetido
  return arbol;
}


static int es_ABB_helper(BTree arbol, int padre, int derecha){
  if(arbol == NULL)
    return 1;
  int padreMenor = padre < arbol->dato;
  if ((derecha && !padreMenor) || (!derecha && padreMenor))
    return 0;
  else{
    return es_ABB_helper(arbol->left, arbol->dato, 0) && 
           es_ABB_helper(arbol->right, arbol->dato, 1);
  } 
}

int es_ABB(BTree arbol){
  if(arbol == NULL)
    return 1;
  return es_ABB_helper(arbol->left, arbol->dato, 0) && 
        es_ABB_helper(arbol->right, arbol->dato, 1);
}

static BTree ABB_detach_menor(BTree arbol, BTree padre, int derecha){
  if (arbol == NULL)
    return padre;
  if(arbol->left)
    return ABB_detach_menor(arbol->left, arbol, 0);
  else {
    BTree *direccionPadre = derecha ? &padre->right : &padre->left;
    *direccionPadre = NULL;
    return arbol;
  }
}

BTree eliminar_ABB(int dato, BTree arbol){
  if (arbol == NULL){
    return arbol;
  }
  if(dato < arbol->dato)
    arbol->left = eliminar_ABB(dato, arbol->left);
  else if(arbol->dato < dato)
    arbol->right = eliminar_ABB(dato, arbol->right);
  else{
    if(arbol->left == NULL && arbol->right == NULL){
      free(arbol);
      return NULL;
    }
    BTree nuevaRaiz = ABB_detach_menor(arbol->right, arbol, 1);
    assert(nuevaRaiz != NULL);
    nuevaRaiz->left = arbol->left;
    nuevaRaiz->right = arbol->right;
    free(arbol);
    return nuevaRaiz;
  }

  return arbol;
}

int cota_inferior_ABB_helper(int n, BTree arbol, int padre){
  if(n < arbol->dato) 
    return cota_inferior_ABB_helper(n, arbol->left, arbol->dato);
  else if(arbol->dato < n){
    if(n < padre)
      return padre;
    else
      return 0; //temp
  }
  else
    return n; //n esta en el arbol
  return 0; // temp
}

int cota_inferior_ABB(int n, BTree arbol){
  
  if(n < arbol->dato)
    return cota_inferior_ABB(n, arbol->right);
  if(arbol->dato < n)
    return cota_inferior_ABB(n, arbol->left);
  return 0; //temp
}

