#ifndef __BSTREE_H__
#define __BSTREE_H__

typedef void *(*FuncionCopiadora)(void *dato);
typedef int (*FuncionComparadora)(void *, void *);
typedef void (*FuncionDestructora)(void *dato);
typedef void (*FuncionVisitanteExtra)(void *dato, void *extra);

typedef enum {
  BTREE_RECORRIDO_IN,  /** Inorden */
  BTREE_RECORRIDO_PRE, /** Preorden */
  BTREE_RECORRIDO_POST /** Postorden */
} BSTreeRecorrido;

struct _BST_Nodo {
  void *dato;
  struct _BST_Nodo *izq, *der;
};


typedef struct _BST_Nodo* BSTree;


BSTree crear_nodo(void *dato, FuncionCopiadora copy);

/**
 * Retorna un arbol de busqueda binaria vacio.
 */
BSTree bstee_crear();

/**
 * Destruye el arbol y sus datos.
 */
void bstree_destruir(BSTree, FuncionDestructora);

/**
 * Retorna 1 si el dato se encuentra y 0 en caso contrario.
 */
int bstree_buscar(BSTree, void *, FuncionComparadora);

/**
 * Inserta un dato no repetido en el arbol, manteniendo la propiedad del arbol
 * de busqueda binaria.
 */
BSTree bstree_insertar(BSTree, void *, FuncionCopiadora, FuncionComparadora);

/**
 * Recorrido DSF del arbol
 */
void bstree_recorrer(BSTree, BSTreeRecorrido, FuncionVisitanteExtra,
                     void *extra);


BSTree bstree_eliminar(BSTree arbol, void *dato, FuncionComparadora comp, FuncionDestructora destroy, FuncionCopiadora copy);

void* bstree_k_esimo_menor(BSTree arbol, int k);
int btree_validar(BSTree arbol, FuncionComparadora comp, void* max, void* min);

#endif //__BSTREE_H__