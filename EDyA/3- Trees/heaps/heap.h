#ifndef ___HEAP_H___
#define ___HEAP_H___

typedef struct _BTNodo {
  void* dato;
  struct _BTNodo *left;
  struct _BTNodo *right;
}BTNodo;

typedef struct _BTNodo *BTree;

typedef void *(*FuncionCopiadora)(void *dato);
typedef int (*FuncionComparadora)(void* dato1, void* dato2);
typedef void (*FuncionDestructora)(void *dato);
typedef void (*FuncionVisitante) (void* dato);

typedef struct _BHeap
{
    void **arr;
    int capacidad;
    int ultimo;
    FuncionComparadora comp;
} *BHeap;

BTree btree_crear();
void btree_destruir(BTree nodo, FuncionDestructora destroy);
int btree_empty(BTree nodo);
BTree btree_unir(void* dato, BTree left, BTree right, FuncionCopiadora copy);


BHeap bheap_crear(int capacidad, FuncionComparadora comp);

void bheap_destruir(BHeap heap, FuncionDestructora destr);

int bheap_vacio(BHeap heap);

void heap_recorrer(BHeap heap, FuncionVisitante visit);

BHeap bheap_insertar(BHeap heap, void* dato, FuncionCopiadora copia);

BHeap bheap_eliminar(BHeap heap);

#endif