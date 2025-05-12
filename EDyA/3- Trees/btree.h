#ifndef __BTREE_H__
#define __BTREE_H__

typedef void (*FuncionVisitante)(int dato);
typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopia)(void *dato);
typedef int (*Predicado) (void *dato);
typedef void (*FuncionVisitanteExtra) (int dato, void *extra);

typedef enum {
  BTREE_RECORRIDO_IN,
  BTREE_RECORRIDO_PRE,
  BTREE_RECORRIDO_POST
} BTreeOrdenDeRecorrido;

typedef struct _BTNodo *BTree;

/**
 * Devuelve un arbol vacío.
 */
BTree btree_crear();

/**
 * Destruccion del árbol.
 */
void btree_destruir(BTree nodo);

/**
 * Indica si el árbol es vacío.
 */
int btree_empty(BTree nodo);

/**
 * Crea un nuevo arbol, con el dato dado en el nodo raiz, y los subarboles dados
 * a izquierda y derecha.
 */
BTree btree_unir(int dato, BTree left, BTree right);

/**
 * Recorrido del arbol, utilizando la funcion pasada.
 */
void btree_recorrer(BTree arbol, BTreeOrdenDeRecorrido orden,
                    FuncionVisitante visit);

void btree_preorder(BTree tree, FuncionVisitante visit);

int btree_nnodos(BTree tree);

int btree_buscar(BTree tree, int dato);

BTree btree_copiar(BTree tree);

int btree_altura(BTree tree);

int btree_nnodos_profundidad(BTree tree, int profundidad);

int btree_profundidad(BTree tree, int dato);
int btree_sumar(BTree tree);

int iesimo(BTree tree, int menor, int* cantNodos);

void santinobisutti(BTree arbolete, FuncionVisitante visit, int level);
void btree_recorrer_bfs(BTree arbol, FuncionVisitante visit);


#endif /* __BTREE_H__ */
