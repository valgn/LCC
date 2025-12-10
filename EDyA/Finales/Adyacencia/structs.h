#ifndef __STRUCTS_H__
#define __STRUCTS_H__

typedef int (*FuncionHasheadora)(int dato);
typedef void (*FuncionVisitante) (int dato);

typedef struct _SNodo {
  int dato;
  struct _SNodo *sig;
} SNodo;

typedef SNodo* SList;

typedef struct {
  SList lista;
} CasillaHash;

/**
 * Estructura principal que representa la tabla hash.
 */
struct _TablaHash {
  CasillaHash *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionHasheadora hash;
};

typedef struct _TablaHash* TablaHash;

SList slist_crear();

void slist_destruir(SList lista);
int slist_vacia(SList lista);
SList slist_agregar_inicio(SList lista, int dato);
void slist_recorrer(SList lista, FuncionVisitante visit);
int slist_buscar(SList lista, int dato);
TablaHash tablahash_crear(unsigned capacidad, FuncionHasheadora hash);
int tablahash_nelems(TablaHash tabla);
int tablahash_capacidad(TablaHash tabla);
void tablahash_destruir(TablaHash tabla);
void tablahash_insertar(TablaHash tabla, int i, int j);
SList slist_eliminar(SList lista, int dato);
void tablahash_borrar(TablaHash tabla, int i, int j);
int es_simetrica(TablaHash tabla);


#endif