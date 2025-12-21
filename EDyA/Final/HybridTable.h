#ifndef ___HYBRIDTABLE_H___
#define ___HYBRIDTABLE_H___

typedef void (*FuncionVisitante) (void* dato);
typedef void *(*FuncionCopiadora)(void *dato);
typedef int (*FuncionComparadora)(void *, void *);
typedef void (*FuncionDestructora)(void *dato);
typedef void (*FuncionVisitanteExtra)(void *dato, void *extra);
typedef unsigned (*FuncionHash)(void *dato);


#define TABLE_CAP 11 // La capacidad de la tabla esta predefinida, elijo un numero primo
#define LIST2AVL 10

typedef enum{RAVL, LIST}
ModoCasilla;

typedef struct {
  void* head; //Puede ser el puntero a una raiz de AVL o una lista enlazada
  int cantidad; // Llevar la cuenta y saber cuando cambiar de lista a avl
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

HybridHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash);


int tablahash_capacidad(HybridHash tabla);

void tablahash_insertar(HybridHash tabla, void *dato);

void* tablahash_buscar(HybridHash tabla, void *dato);

void tablahash_eliminar(HybridHash tabla, void *dato);
void tabla_destruir(HybridHash tabla);
#endif