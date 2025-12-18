#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef void *(*FuncionCopiadora)(void *dato);
/** Retorna una copia fisica del dato */
typedef int (*FuncionComparadora)(void *dato1, void *dato2);
/** Retorna un entero negativo si dato1 < dato2, 0 si son iguales y un entero
 * positivo si dato1 > dato2  */
typedef void (*FuncionDestructora)(void *dato);
/** Libera la memoria alocada para el dato */
typedef unsigned (*FuncionHash)(void *dato);
/** Retorna un entero sin signo para el dato */
typedef void (*Impresora)(void*);

typedef enum{
    OCUPADA,
    VACIA,
    BORRADA
}EstadoCasilla;

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  void *dato;
  EstadoCasilla estado;
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
    tabla->elems[idx].estado = VACIA;
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
    if (tabla->elems[idx].dato == OCUPADA)
      tabla->destr(tabla->elems[idx].dato);

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}




/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */
void tablahash_insertar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
    unsigned idx = tabla->hash(dato) % tabla->capacidad;
    unsigned i = idx; int espacio = -1; int lugar = 0;
    do{
        if(tabla->elems[idx].estado == OCUPADA){
            if(tabla->comp(tabla->elems[idx].dato, dato) == 0){
                tabla->destr(tabla->elems[idx].dato);
                tabla->elems[idx].dato = tabla->copia(dato);
                return;
            }
        }
        else if(tabla->elems[idx].estado == VACIA){
            if(espacio == -1){
                espacio = idx; lugar = 1;
            }
        }
        else if(tabla->elems[idx].estado == BORRADA){
            if(espacio == -1) espacio = idx;
        }
        idx = (idx+1) % tabla->capacidad;
    }while(i != idx && !lugar);

    if(espacio != -1){
        tabla->elems[espacio].dato = tabla->copia(dato);
        tabla->elems[espacio].estado = OCUPADA;
        tabla->numElems++;
    }
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  int i = idx;
  do{
    if(tabla->elems[idx].estado == VACIA){
        return NULL;
    }
    if(tabla->elems[idx].estado == OCUPADA){
        if(tabla->comp(tabla->elems[idx].dato, dato)==0){
            return tabla->elems[idx].dato;
        }
    }
    idx = (idx+1)%tabla->capacidad;
  }while(i != idx);
  return NULL;
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  int i = idx;
  do{
    if(tabla->elems[idx].estado == VACIA) return;
    if(tabla->elems[idx].estado == OCUPADA){
        if(tabla->comp(tabla->elems[idx].dato, dato) == 0){
            tabla->destr(tabla->elems[idx].dato);
            tabla->elems[idx].estado == BORRADA; tabla->numElems--;
        }
    }
    idx = (idx+1) % tabla->capacidad;   
}while(idx != i);
}


void tablahash_imprimir(TablaHash tabla, Impresora f){
    for(int i = 0; i<tabla->capacidad; i++){
        if(tabla->elems[i].estado == OCUPADA){
            f(tabla->elems[i].dato);
        }
    }
}

int comparar_enteros(void* d1, void* d2){
    return *(int*)d1 - *(int*)d2;
}
void* copiar_entero(void* dato){
    int* val = malloc(sizeof(int));
    *val = *(int*)dato;
    return val;
}
void imprimir_entero(void* dato){
    printf("%d ", *(int*)dato);
}
void destruir_entero(void* dato){
    free(dato);
}
unsigned hashear_entero(void* dato){
    return *(int*)dato+1;
}


int main(){
    TablaHash tabla = tablahash_crear(6, copiar_entero, comparar_enteros, destruir_entero, hashear_entero);
    int numero = 3;
    for(int i = 0; i<tabla->capacidad; i++){
        numero = (numero*i)+1;
        tablahash_insertar(tabla, &numero);
    }
    tablahash_imprimir(tabla, imprimir_entero);
    tablahash_destruir(tabla);
    return 0;
}