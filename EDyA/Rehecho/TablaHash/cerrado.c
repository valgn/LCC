#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


typedef void *(*FuncionCopiadora)(void *dato);
/** Retorna una copia fisica del dato */
typedef int (*FuncionComparadora)(void *dato1, void *dato2);
/** Retorna un entero negativo si dato1 < dato2, 0 si son iguales y un entero
 * positivo si dato1 > dato2  */
typedef void (*FuncionDestructora)(void *dato);
/** Libera la memoria alocada para el dato */
typedef unsigned (*FuncionHash)(void *dato);
/** Retorna un entero sin signo para el dato */
typedef void (*FuncionVisitante)(void *dato);

typedef enum{
    taken,
    freed,
    removed,
}BoxState;

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  void *dato;
  BoxState state; 
} CasillaHash;

/**
 * Estructura principal que representa la tabla hash.
 */
struct _TablaHash {
  CasillaHash *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};
typedef struct _TablaHash *TablaHash;

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash) {

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(CasillaHash) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->destr = destr;
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx].dato = NULL;
    tabla->elems[idx].state = freed;
  }
  return tabla;
}

/**
 * Retorna el numero de elementos de la tabla.
 */
int tablahash_nelems(TablaHash tabla) { return tabla->numElems; }

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla) { return tabla->capacidad; }

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla) {

  // Destruir cada uno de los datos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx)
    if (tabla->elems[idx].state == taken)
      tabla->destr(tabla->elems[idx].dato);

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}

void rehashing(TablaHash tabla){
    int newCap = tabla->capacidad * 2;
    CasillaHash* newElems = malloc(sizeof(CasillaHash) * newCap);
    for(int i = 0; i<newCap;i++){
        newElems[i].dato = NULL;
        newElems[i].state = freed;
    }
    for(int idx = 0; idx<tabla->capacidad; idx++){
        if(tabla->elems[idx].state == taken){
            unsigned newIdx = tabla->hash(tabla->elems[idx].dato) % newCap;
            void* copia_dato = tabla->elems[idx].dato;
            while(newElems[newIdx].state == taken) newIdx = (newIdx + 1) % newCap;
            newElems[newIdx].dato = copia_dato;
            newElems[newIdx].state = taken;
        }
    }
    free(tabla->elems);
    tabla->elems = newElems; tabla->capacidad = newCap;
}


/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */
void tablahash_insertar(TablaHash tabla, void *dato) {
  if ((float)tabla->numElems / tabla->capacidad >= 0.7) {
        rehashing(tabla);
    }  

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  int i = idx, libre = 0, borrado = -1;
  do{
    if(tabla->elems[idx].state == freed){
        if(borrado != -1) idx = borrado;
        tabla->elems[idx].dato = tabla->copia(dato);
        tabla->numElems++;
        tabla->elems[idx].state = taken;
        libre = 1;
    }
    else if(tabla->elems[idx].state == removed)
        if(borrado == -1) borrado = idx;

    else if(tabla->elems[idx].state == taken){
        if(tabla->comp(tabla->elems[idx].dato, dato) == 0){
            tabla->destr(tabla->elems[idx].dato); tabla->elems[idx].dato = tabla->copia(dato);
            return;
        }
    }
    idx = (idx+1) % tabla->capacidad;
  }while(i != idx && !libre);
  if(borrado != -1){
    tabla->elems[borrado].dato = tabla->copia(dato);
    tabla->numElems++;
    tabla->elems[borrado].state = taken;
    return;
  }
}


/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void* tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  int i = idx;
  do{
    if(tabla->elems[idx].state == taken){
        if(tabla->comp(tabla->elems[idx].dato, dato) == 0) return tabla->elems[idx].dato;
    }
    else if(tabla->elems[idx].state == freed) return NULL;
    idx = (idx + 1) % tabla->capacidad;
  }while(i != idx);
return NULL;
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  int i = idx, borrado = 0;
  do{
    if(tabla->elems[idx].state == taken){
        if(tabla->comp(tabla->elems[idx].dato,dato) == 0){
            tabla->destr(tabla->elems[idx].dato);
            tabla->elems[idx].state = removed; tabla->numElems--; borrado = 1;
        }
    }
    else if(tabla->elems[idx].state == removed) idx = (idx+1)%tabla->capacidad;
    else return;
  }
  while(i != idx && !borrado);
  return;
}

void printHash(TablaHash tabla, FuncionVisitante visit){
    for(int i = 0; i < tabla->capacidad; i++){
        if(tabla->elems[i].state == taken){
            printf("La casilla %d tiene la lista conformada por: ", i);
            visit(tabla->elems[i].dato);
            puts("");
        }
     }
}

void destruir_entero(void* dato){
    free(dato);
}
void* copiar_entero(void* dato){
    int* val = malloc(sizeof(int));
    assert(val);
    *val = *(int*)dato;
    return val;
}

void imprimir_entero(void* dato){
    printf("%d ", *(int*)dato);
}
int comparar_entero(void* dato1, void* dato2){
    return *(int*)dato1 - *(int*)dato2;
}
unsigned hashear_entero(void* dato){
    return *(int*)dato;
}

int main(){
    TablaHash hashTable = tablahash_crear(10, copiar_entero, comparar_entero, destruir_entero, hashear_entero);
    printf("---CAPACIDAD DE LA TABLA: %d---\n", hashTable->capacidad);
    for(int i = 0; i < 9; i++){
        tablahash_insertar(hashTable, &i);
    }
    int cinco = 5; //tablahash_insertar(hashTable, &cinco);
    int cien = 100;
    printHash(hashTable, imprimir_entero);
    printf("---CAPACIDAD DE LA TABLA: %d---\n", hashTable->capacidad);
    puts("");
    
   for(int i = 3; i<7;i++) tablahash_eliminar(hashTable, &i);
    
    printHash(hashTable, imprimir_entero);
    void* buscado = tablahash_buscar(hashTable, &cien);
    if(buscado != NULL) imprimir_entero(buscado);
    else printf("No se encontro el dato\n");

    tablahash_destruir(hashTable);
    return 0;
}