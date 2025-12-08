#include "btree.h"
#include <assert.h>
#include <stdlib.h>

typedef struct _BTNodo
{
  int dato;
  struct _BTNodo *left;
  struct _BTNodo *right;
} BTNodo;

// PILAS GENERALES
typedef struct _PNodo
{
  void *dato;
  struct _PNodo *sig;
} Nodo;

typedef struct
{
  Nodo *tope;
} Pila;

// Cola rep
typedef struct _ColaRep
{
  Nodo *frente;
  Nodo *ultimo;
} *Cola;

// Destruir nodo
void nodestruye(void* dato)
{

  return;
}

Cola cola_crear()
{
  Cola c = malloc(sizeof(struct _ColaRep));
  c->frente = c->ultimo = NULL;
  return c;
}

int cola_vacia(Cola cola){
  return cola->frente == NULL;
}

void cola_destruir(Cola cola, FuncionDestructora destroy)
{
    Nodo* actual = cola->frente;
    while(actual){
    Nodo* temp = actual->sig;
    destroy(actual->dato);
    free(actual);
    actual = temp;
  }
  free(cola);
}

void enqueue(Cola cola, void* dato){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->dato = dato;
  nodo->sig = NULL;
  if(cola_vacia(cola)){
    cola->frente = cola->ultimo = nodo;
  }
  else{
    cola->ultimo->sig = nodo;
    cola->ultimo = nodo;
  }
}

void* dequeue(Cola cola){
  if(cola->frente == NULL){
    return NULL;
  }
  Nodo* temp = cola->frente;
  void* dato = temp->dato;
  cola->frente = cola->frente->sig;
  if(cola->frente == NULL){
    cola->ultimo = NULL;
  }
  free(temp);
  return dato;
}

Pila *pila_crear()
{
  Pila *pila = malloc(sizeof(Pila));
  pila->tope = NULL;
  return pila;
}

void pila_push(Pila *pila, void *dato)
{
  Nodo *nuevo = malloc(sizeof(Nodo));
  nuevo->dato = dato;
  nuevo->sig = pila->tope;
  pila->tope = nuevo;
}

void *pila_pop(Pila *pila)
{
  if (pila->tope == NULL)
    return NULL;

  Nodo *nodo = pila->tope;
  void *dato = nodo->dato;
  pila->tope = nodo->sig;
  free(nodo);
  return dato;
}

int pila_es_vacia(Pila *pila)
{
  return pila->tope == NULL;
}

void pila_destruir(Pila *pila, FuncionDestructora destroy)
{
  Nodo* actual = pila->tope;
  while(actual){
    Nodo* temp = actual->sig;
    destroy(actual->dato);
    free(actual);
    actual = temp;
  }
  free(pila);
}

/**
 * Devuelve un arbol vacío.
 */
BTree btree_crear() { return NULL; }

/**
 * Destruccion del árbol.
 */
void btree_destruir(BTree nodo)
{
  if (nodo != NULL)
  {
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
BTree btree_unir(int dato, BTree left, BTree right)
{
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
void btree_recorrer(BTree arbol, BTreeOrdenDeRecorrido orden, FuncionVisitante visit)
{
  if (arbol == NULL)
  {
    return;
  }
  if (orden == BTREE_RECORRIDO_PRE)
    visit(arbol->dato);
  btree_recorrer(arbol->left, orden, visit);
  if (orden == BTREE_RECORRIDO_IN)
    visit(arbol->dato);
  btree_recorrer(arbol->right, orden, visit);
  if (orden == BTREE_RECORRIDO_POST)
    visit(arbol->dato);
}

void btree_preorder(BTree tree, FuncionVisitante visit)
{
  Pila *pila = pila_crear();
  if (tree == NULL)
  {
    return;
  }
  pila_push(pila, tree);

  while (!pila_es_vacia(pila))
  {
    BTNodo *actual = (BTNodo *)pila_pop(pila);
    visit(actual->dato);
    if (actual->right != NULL)
    {
      pila_push(pila, actual->right);
    }
    if (actual->left != NULL)
    {
      pila_push(pila, actual->left);
    }
  }
  pila_destruir(pila, (FuncionDestructora)nodestruye);
}

int btree_nnodos(BTree tree)
{
  if (tree == NULL)
  {
    return 0;
  }
  return 1 + btree_nnodos(tree->left) + btree_nnodos(tree->right);
}

int btree_buscar(BTree tree, int dato)
{
  if (tree == NULL)
  {
    return 0;
  }
  if (tree->dato == dato)
  {
    return 1;
  }
  return btree_buscar(tree->left, dato) || btree_buscar(tree->right, dato);
}

BTree btree_copiar(BTree tree)
{
  if (tree == NULL)
  {
    return NULL;
  }
  BTNodo *copia = malloc(sizeof(BTNodo));
  *copia = *tree;
  copia->dato = tree->dato;
  copia->left = btree_copiar(tree->left);
  copia->right = btree_copiar(tree->right);
  return copia;
}

int btree_altura(BTree tree)
{
  if (tree == NULL)
  {
    return 0;
  }
  int sumaizq = 1 + btree_altura(tree->left);
  int sumader = 1 + btree_altura(tree->right);
  if (sumaizq < sumader)
  {
    return sumader;
  }
  else
    return sumaizq;
}

int btree_nnodos_profundidad(BTree tree, int profundidad)
{
  if (tree == NULL)
  {
    return 0;
  }
  if (profundidad == 0)
  {
    return 1;
  }
  return btree_nnodos_profundidad(tree->left, profundidad - 1) + btree_nnodos_profundidad(tree->right, profundidad - 1);
}

int btree_profundidad(BTree tree, int dato)
{
  if (tree == NULL)
  {
    return -1;
  }
  if (tree->dato == dato)
  {
    return 0;
  }

  int profundidad = btree_profundidad(tree->left, dato);
  if (profundidad != -1)
    return 1 + profundidad;

  int profundidad2 = btree_profundidad(tree->right, dato);
  if (profundidad2 != -1)
    return 1 + profundidad2;

  return -1;
}

int btree_sumar(BTree tree)
{
  if (tree == NULL)
  {
    return 0;
  }
  return tree->dato + btree_sumar(tree->left) + btree_sumar(tree->right);
}

int iesimo(BTree tree, int menor, int *cantNodos)
{
  if (tree == NULL)
  {
    return 0; // Si el nodo es NULL, no hay nada que hacer
  }

  // Recorrer el subárbol izquierdo
  int left = iesimo(tree->left, menor, cantNodos);

  // Si ya encontramos el valor, lo retornamos
  if (left != 0)
  {
    return left;
  }

  // Incrementar el contador al procesar el nodo actual
  (*cantNodos)++;

  // Si el contador alcanza 'menor', hemos encontrado el nodo
  if (*cantNodos == menor)
  {
    return tree->dato; // Retornar el valor del nodo
  }

  // Recorrer el subárbol derecho
  return iesimo(tree->right, menor, cantNodos);
}

void santinobisutti(BTree arbolete, FuncionVisitante visit, int level)
{
  if (arbolete == NULL)
  {
    return;
  }
  if (level == 0)
  {
    visit(arbolete->dato);
  }
  else
  {
    santinobisutti(arbolete->left, visit, level - 1);
    santinobisutti(arbolete->right, visit, level - 1);
  }
}

void btree_recorrer_bfs(BTree arbol, FuncionVisitante visit)
{
  int altura = btree_altura(arbol);
  for (int i = 0; i < altura; i++)
  {
    santinobisutti(arbol, visit, i);
  }
}

BTree mirror(BTree arbol)
{
  if (arbol == NULL)
  {
    return NULL;
  }
  BTree espejin = malloc(sizeof(struct _BTNodo));
  espejin->dato = arbol->dato;

  espejin->left = mirror(arbol->right);
  espejin->right = mirror(arbol->left);

  return espejin;
}

void bfs(BTree arbol, FuncionVisitante visit){
  
  if(arbol == NULL){
    return NULL;
  }
  Cola c = cola_crear();
  enqueue(c, arbol);
  while(!cola_vacia(c)){
    BTNodo* nodo = (BTNodo*)dequeue(c);
    visit(nodo->dato);
    if(nodo->left != NULL){
      enqueue(c, nodo->left);
    }
    if(nodo->right != NULL){
      enqueue(c, nodo->right);
    }
  }
  cola_destruir(c, (FuncionDestructora)nodestruye);
}
