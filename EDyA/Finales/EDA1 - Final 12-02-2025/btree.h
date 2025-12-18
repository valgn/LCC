#ifndef __BTREE_H__
#define __BTREE_H__

typedef void (*FuncionVisitante)(int dato);

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
                    

/**
 * dado un árbol binario, retorne 1 si además es de búsqueda o 0 si no lo es
 */
int es_ABB(BTree arbol);

BTree btree_insertar_random(int dato,BTree arbol);

BTree ABB_insertar(int dato, BTree arbol);

/**
 * Elimina dato de arbol y devuelve el resultado
 */
BTree eliminar_ABB(int dato, BTree arbol);

/**
 * Toma un árbol binario de búsqueda y un valor y devuelve el mı́nimo elemento 
 * presente en el árbol que sea mayor o igual al valor dado
 */

void btree_imprimir(BTree arbol);

int cota_inferior_ABB(int n, BTree arbol);

#endif /* __BTREE_H__ */
