#include <stdio.h>
#include <stdlib.h>

typedef void *(*FuncionCopiadora)(void *dato);
typedef int (*FuncionComparadora)(void *dato1, void *dato2);
typedef void (*FuncionDestructora)(void *dato);
typedef unsigned (*FuncionHash)(void *dato);
typedef void (*FuncionVisitante)(void *dato);

typedef struct _AVL_Nodo {
  void* dato;
  struct _AVL_Nodo* izq, * der;
  int altura;
} AVL_Nodo;


typedef struct {
  AVL_Nodo *raiz;
} CasillaHash;

struct _TablaHash {
  CasillaHash *elems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

typedef struct _TablaHash *TablaHash;

TablaHash tablahash_crear (unsigned capacidad, FuncionComparadora comp, FuncionCopiadora copy, FuncionDestructora destroy, FuncionHash hash) {
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  tabla->elems = malloc(sizeof(CasillaHash)*capacidad);
  tabla->capacidad = capacidad;
  tabla->comp = comp;
  tabla->copia = copy;
  tabla->destr = destroy;
  tabla->hash = hash;

  for (unsigned idx = 0 ; idx < tabla->capacidad ; ++idx)
    tabla->elems[idx].raiz = NULL;

  return tabla;
}

int avl_nodo_altura (AVL_Nodo *raiz) {
  return raiz == NULL ? -1 : raiz->altura; 
}

unsigned int avl_nodo_max_altura_hijos(AVL_Nodo* raiz) {
  int alturaIzq = avl_nodo_altura(raiz->izq);
  int alturaDer = avl_nodo_altura(raiz->der);
  return (alturaIzq < alturaDer ? alturaDer : alturaIzq);
}

int avl_nodo_factor_balance(AVL_Nodo* raiz) {
  int factor = avl_nodo_altura(raiz->der) - avl_nodo_altura(raiz->izq);
  return factor;
}

AVL_Nodo* avl_nodo_rotacion_simple_izq(AVL_Nodo* raiz) {
  AVL_Nodo* hijoDer = raiz->der;
  raiz->der = hijoDer->izq;
  hijoDer->izq = raiz;
  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
  hijoDer->altura = 1 + avl_nodo_max_altura_hijos(hijoDer);
  return hijoDer;
}

AVL_Nodo* avl_nodo_rotacion_simple_der (AVL_Nodo *raiz) {
  AVL_Nodo *hijoIzq = raiz->izq;
  raiz->izq = hijoIzq->der;
  hijoIzq->der = raiz;
  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
  hijoIzq->altura = 1 + avl_nodo_max_altura_hijos(hijoIzq);
  return hijoIzq;
}

AVL_Nodo* avl_rebalancear (AVL_Nodo *raiz) {
  if (avl_nodo_factor_balance(raiz) == -2) { // izq
    if (avl_nodo_factor_balance(raiz->izq) == 1) //der
      raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
    raiz = avl_nodo_rotacion_simple_der(raiz);
  } else if (avl_nodo_factor_balance(raiz) == 2) { //der
    if (avl_nodo_factor_balance(raiz->der) == -1) //izq
      raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
    raiz = avl_nodo_rotacion_simple_izq(raiz);
  }
  return raiz;
}

AVL_Nodo* avl_nodo_crear(void* dato, FuncionCopiadora copia) {
  AVL_Nodo* nuevoNodo = malloc(sizeof(AVL_Nodo));
  nuevoNodo->dato = copia(dato);
  nuevoNodo->izq = nuevoNodo->der = NULL;
  nuevoNodo->altura = 0;
  return nuevoNodo;
}

AVL_Nodo* avl_nodo_insertar(AVL_Nodo* raiz, void* dato, FuncionCopiadora copia, FuncionComparadora comp) {
  if (raiz == NULL)
    return avl_nodo_crear(dato, copia);

  if (comp(dato, raiz->dato) < 0)
    raiz->izq = avl_nodo_insertar(raiz->izq, dato, copia, comp);

  else if (comp(dato, raiz->dato) > 0)
    raiz->der = avl_nodo_insertar(raiz->der, dato, copia, comp);

  else
    return raiz;

  raiz = avl_rebalancear(raiz);
  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
  return raiz;
}

// FALTA FACTOR BALANCE QUE NO LO HAGO AHORA PORQUE ELIMINE EL NUMELEMS

void tablahash_insertar (TablaHash tabla, void *dato) {
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  int bandera = 0;

  if (tabla->elems[idx].raiz == NULL) {
    tabla->elems[idx].raiz = avl_nodo_crear(dato, tabla->copia);
    return;
  }

  AVL_Nodo *act = tabla->elems[idx].raiz;
  while (act != NULL && !bandera) {
    int comparacion = tabla->comp(dato, act->dato);
    if (comparacion == 0) {
      tabla->destr(act->dato);
      act->dato = tabla->copia(dato);
      bandera = 1;
    } else
      act = (comparacion < 0 ? act->izq : act->der);
  }
  if (!bandera)
    tabla->elems[idx].raiz = avl_nodo_insertar(tabla->elems[idx].raiz, dato, tabla->copia, tabla->comp);
}

AVL_Nodo * avl_nodo_eliminar (AVL_Nodo *raiz, void *dato, FuncionComparadora comp, FuncionDestructora dest, FuncionCopiadora copy) {
  if (raiz == NULL) return NULL;

  if (comp(dato, raiz->dato) < 0)
    raiz->izq = avl_nodo_eliminar(raiz->izq, dato, comp, dest, copy);
  else if (comp(dato, raiz->dato) > 0)
    raiz->der = avl_nodo_eliminar(raiz->der, dato, comp, dest, copy);
  else {

    if (raiz->izq == NULL && raiz->der == NULL) {
      dest(raiz->dato);
      free(raiz);
      return NULL;
    }

    if (raiz->der == NULL) {
      AVL_Nodo *temp = raiz->izq;
      dest(raiz->dato);
      free(raiz);
      return temp;
    }

    if (raiz->izq == NULL) {
      AVL_Nodo *temp = raiz->der;
      dest(raiz->dato);
      free(raiz);
      return temp;
    }

    AVL_Nodo *act = raiz->der;
    while(act->izq != NULL)
      act = act->izq;

    dest(raiz->dato);
    raiz->dato = copy(act->dato);

    raiz->der = avl_nodo_eliminar(raiz->der, act->dato, comp, dest, copy);
  }

  raiz = avl_rebalancear(raiz);
  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);

  return raiz;
}

void tablahash_eliminar (TablaHash tabla, void *dato) {
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  tabla->elems[idx].raiz= avl_nodo_eliminar(tabla->elems[idx].raiz, dato, tabla->comp, tabla->destr, tabla->copia);
}

void avl_rehash (AVL_Nodo *raiz, CasillaHash *nuevaCasilla, unsigned nuevaCapacidad, FuncionComparadora comp, FuncionDestructora dest, FuncionCopiadora copy, FuncionHash hash) {
  if (raiz == NULL) return;

  avl_rehash (raiz->izq, nuevaCasilla, nuevaCapacidad, comp, dest, copy, hash);

  unsigned idx = hash(raiz->dato) % nuevaCapacidad;
  nuevaCasilla[idx].raiz = avl_nodo_insertar(nuevaCasilla[idx].raiz, raiz->dato, copy, comp);

  avl_rehash (raiz->der, nuevaCasilla, nuevaCapacidad, comp, dest, copy, hash);
}

void avl_destruir(AVL_Nodo *raiz, FuncionDestructora destr) {
    if (raiz == NULL) return;

    avl_destruir(raiz->izq, destr);
    avl_destruir(raiz->der, destr);

    destr(raiz->dato);
    free(raiz);
}

void tablahash_redimensionar (TablaHash tabla) {
  unsigned nuevaCapacidad = tabla->capacidad * 2;
  CasillaHash *nuevoElems = malloc(sizeof(CasillaHash)*nuevaCapacidad);

  for (unsigned i = 0 ; i < nuevaCapacidad ; ++i)
    nuevoElems[i].raiz = NULL;

  for (unsigned i = 0; i < tabla->capacidad ; ++i) {
    AVL_Nodo *nodo = tabla->elems[i].raiz;
    avl_rehash(nodo, nuevoElems, nuevaCapacidad, tabla->comp, tabla->destr, tabla->copia, tabla->hash);
  }

  for (unsigned i = 0 ; i < tabla->capacidad ; ++i)
    avl_destruir(tabla->elems[i].raiz, tabla->destr);

  free(tabla->elems);
  tabla->elems = nuevoElems;
  tabla->capacidad = nuevaCapacidad;
}

void tablahash_destruir(TablaHash tabla) {
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx)
    if (tabla->elems[idx].raiz != NULL)
      avl_destruir(tabla->elems[idx].raiz, tabla->destr);

  free(tabla->elems);
  free(tabla);
  return;
}

/* ---- funciones para int ---- */

void *copiar_entero(void *dato) {
    int *p = malloc(sizeof(int));
    *p = *(int *)dato;
    return p;
}

int comparar_entero(void *a, void *b) {
    return *(int *)a - *(int *)b;
}

void destruir_entero(void *a) {
    free(a);
}

unsigned hash_entero(void *dato) {
    return *(unsigned *)dato;
}

/* ---- incluye tu .c con AVL + TablaHash acá ---- */
/* por ejemplo:
#include "tabla_hash_avl.c"
*/

/* ---- función para imprimir un nodo ---- */
void imprimir_entero(void *dato) {
    printf("%d ", *(int *)dato);
}

/* ------ main de prueba ------ */

int main() {

    TablaHash tabla = tablahash_crear(
        5,                 // capacidad inicial chica para forzar rebalanceos
        comparar_entero,
        copiar_entero,
        destruir_entero,
        hash_entero
    );

    printf("Insertando elementos...\n");

    for (int i = 0; i < 20; i++) {
        tablahash_insertar(tabla, &i);
    }

    printf("Buscando algunos elementos...\n");
    for (int i = 0; i < 20; i += 3) {
        unsigned idx = hash_entero(&i) % tabla->capacidad;
        AVL_Nodo *nodo = tabla->elems[idx].raiz;

        printf("Bucket %u contiene %d ? -> ", idx, i);

        AVL_Nodo *act = nodo;
        int encontrado = 0;
        while (act != NULL && !encontrado) {
            int c = comparar_entero(&i, act->dato);
            if (c == 0) encontrado = 1;
            else act = c < 0 ? act->izq : act->der;
        }

        printf(encontrado ? "SI\n" : "NO\n");
    }

    printf("\nEliminando algunos elementos...\n");
    for (int i = 0; i < 20; i += 4) {
        printf("Eliminando %d...\n", i);
        tablahash_eliminar(tabla, &i);
    }

    printf("\nRedimensionando tabla...\n");
    tablahash_redimensionar(tabla);

    printf("Insertando más elementos tras redimensionar...\n");
    for (int i = 30; i < 40; i++) {
        tablahash_insertar(tabla, &i);
    }

    printf("\nChequeando existencia tras eliminar y rehash...\n");
    for (int i = 0; i < 40; i++) {
        unsigned idx = hash_entero(&i) % tabla->capacidad;
        AVL_Nodo *act = tabla->elems[idx].raiz;

        int encontrado = 0;
        while (act != NULL && !encontrado) {
            int c = comparar_entero(&i, act->dato);
            if (c == 0) encontrado = 1;
            else act = c < 0 ? act->izq : act->der;
        }

        printf("%2d: %s\n", i, encontrado ? "OK" : "NO");
    }

    printf("\nTEST FINALIZADO (usá valgrind para verificar memoria)\n");

    /* destruir la tabla entera */
    for (unsigned i = 0; i < tabla->capacidad; i++)
        avl_destruir(tabla->elems[i].raiz, destruir_entero);

    free(tabla->elems);
    free(tabla);

    return 0;
}