#include <stdlib.h>
#include <stdio.h>
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

typedef struct _Nodo{
    void* dato;
    double gp;
    struct _Nodo* sig;
}Nodo;
typedef Nodo* Lista;

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */


/**
 * Estructura principal que representa la tabla hash.
 */
struct _CD {
    Lista *elems;
    unsigned numElems;
    unsigned capacidad;
    FuncionCopiadora copia;
    FuncionComparadora comp;
    FuncionDestructora destr;
    FuncionHash hash;
};
typedef struct _CD *CD;

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
CD CD_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash) {

  // Pedimos memoria para la estructura principal y las casillas.
  CD tabla = malloc(sizeof(struct _CD));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(Lista) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->destr = destr;
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx] = NULL;
  }

  return tabla;
}

/**
 * Retorna el numero de elementos de la tabla.
 */
int CD_nelems(CD tabla) { return tabla->numElems; }

/**
 * Retorna la capacidad de la tabla.
 */
int CD_capacidad(CD tabla) { return tabla->capacidad; }

/**
 * Destruye la tabla.
 */
void CD_destruir(CD tabla) {

  // Destruir cada uno de los datos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx){
      Nodo* temp = tabla->elems[idx];
      while(temp != NULL){
        Nodo* borrar = temp;
        temp = temp->sig;
        tabla->destr(borrar->dato);
        free(borrar);
      }
    }


  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}


Lista insertar_inicio(Lista lista, void* dato, double gp, FuncionCopiadora copy){
    Nodo* nuevoNodo = malloc(sizeof(Nodo));
    nuevoNodo->dato = copy(dato); nuevoNodo->gp = gp;
    nuevoNodo->sig = lista;
    lista = nuevoNodo;
    return lista;
}

Nodo* devolver_nodo(Lista lista, void* dato, FuncionComparadora cmp){
    Nodo* temp = lista;
    while(temp != NULL && cmp(temp->dato, dato) != 0) temp = temp->sig;
    return temp;
}


/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */
void CD_insertar(CD tabla, void *dato, double gp) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  Nodo* buscado = devolver_nodo(tabla->elems[idx], dato, tabla->comp);
  // Insertar el dato si la casilla estaba libre.
  if(gp == 0) return;
  if(!buscado){
    tabla->elems[idx] = insertar_inicio(tabla->elems[idx], dato, gp, tabla->copia);
    tabla->numElems++;
  }
  else{
    tabla->destr(buscado->dato); buscado->dato = tabla->copia(dato);
    buscado->gp = gp;
  }
  return;
}

Nodo* devolver_nodo_y_anterior(Lista lista, void* dato, FuncionComparadora cmp, Nodo** ant){
    Nodo* temp = lista;
    while(temp != NULL && cmp(temp->dato, dato) != 0){
        *ant = temp;
        temp = temp->sig;
    } 
    return temp;
}

void CD_modificar_gp(CD set, void* dato, double gp){
    unsigned idx = set->hash(dato) % set->capacidad;
    Nodo* ant = NULL;
    Nodo* buscado = devolver_nodo_y_anterior(set->elems[idx], dato, set->comp, &ant);
    if(gp == 0){
        if(buscado == NULL) return;
        if(ant == NULL) set->elems[idx] = set->elems[idx]->sig;
        else{
            ant->sig = buscado->sig;
        }
        set->numElems--;
        set->destr(buscado->dato);
        free(buscado);
    }
    else buscado->gp = gp;
    return;
}

int cd_subconjunto(CD a, CD b){
    for(int i = 0; i < a->capacidad; i++){
        Nodo* temp = a->elems[i];
        while(temp != NULL){
            unsigned idx = b->hash(temp->dato) % b->capacidad;
            Nodo* elemento_b = devolver_nodo(b->elems[idx], temp->dato, b->comp);
            if(elemento_b == NULL) return 0;
            else if((temp->gp - elemento_b->gp) > 0) return 0;
            temp = temp->sig;
        }
    }
    return 1;
}

CD cd_union(CD a, CD b){
    CD res = CD_crear(a->capacidad + b->capacidad, a->copia, a->comp, a->destr, a->hash);
    for(int i = 0; i < a->capacidad; i++){
        Nodo* temp = a->elems[i];
        while(temp != NULL){
            CD_insertar(res, temp->dato, temp->gp);
            temp = temp->sig;
        }
    }
    for(int i = 0; i < b->capacidad; i++){
        Nodo* temp = b->elems[i];
        while(temp != NULL){
            unsigned idx = res->hash(temp->dato) % res->capacidad;
            Nodo* buscado = devolver_nodo(res->elems[idx]);
            if(!buscado) CD_insertar(res, temp->dato, temp->gp);
            else CD_modificar_gp(res, temp->dato, temp->gp); 
            temp = temp->sig;
        }
    }
    return res;
}

CD CD_diferencia(CD a, CD b){
    CD res = CD_crear(a->capacidad, a->copia, a->comp, a->destr, a->hash);
    for(int i = 0; i < a->capacidad; i++){
        Nodo* temp = a->elems[i];
        while(temp != NULL){
            unsigned b_idx = b->hash(temp->dato) % b->capacidad;
            Nodo* buscado = devolver_nodo(b->elems[b_idx], temp->dato, a->comp);
            if(buscado){
                if(temp->gp >= buscado->gp) CD_insertar(res, temp->dato, temp->gp - buscado->gp);
                else CD_insertar(res, temp->dato, 0);
            }
            temp = temp->sig;
        }
    }
    return res;
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *CD_retornar(CD tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  Nodo* buscado = devolver_nodo(tabla->elems[idx], dato, tabla->comp);
  if(buscado == NULL) return NULL;
  else return buscado;
}


void* copiar_entero(void* dato){
    int* val = malloc(sizeof(int));
    assert(val != NULL);
    *val = *(int*)dato;
    return val;
}

void destruir_entero(void* dato){
    free(dato);
}

unsigned hashear_entero(void* dato){
    return *(int*)dato;
}

int comparar_enteros(void* dato1, void* dato2){
    return *(int*)dato1 - *(int*)dato2;
}

void imprimir_entero(void* dato){
    printf("%d ", *(int*)dato);
}

void imprimir_set_int(CD set, void imprimir(void*)){
    for(int i = 0; i < set->capacidad; i++){
        if(set->elems[i] != NULL){
            Nodo* temp = set->elems[i];
            while(temp != NULL){
                imprimir(temp->dato);
                temp = temp->sig;
            }
        }
    }
}


int main(){
    CD set = CD_crear(10, copiar_entero, comparar_enteros, destruir_entero, hashear_entero);
    for(int i = 0; i < set->capacidad; i++){
        CD_insertar(set, &i, (double)i/set->capacidad);
    }
    // imprimir_set_int(set, imprimir_entero);
    // Veamos el grado de el numero 9 insertado. Tendria que ser 0.9
    int valor_buscado = 9;
    printf("BUSCAMOS EL 9 Y VEMOS SU GRADO.\n");
    Nodo* buscado = CD_retornar(set, &valor_buscado);
    printf("Testeando...\n");
    assert(buscado->gp == 0.9);
    printf("Test pasado correctamente.\n");
    puts("");
    printf("Cambiemos el grado del 9 a otro valor, 0.5\n");
    CD_modificar_gp(set, &valor_buscado, 0.5);
    printf("Testeando si el grado es 0.5...\n");
    assert(buscado->gp == 0.5);
    printf("Test pasado correctamente.\n");

    puts("");

    CD set2 = CD_crear(5, copiar_entero, comparar_enteros, destruir_entero, hashear_entero);
    for(int i = 2; i < set2->capacidad+2; i++){
        CD_insertar(set2, &i, (double)i/11);
    }
    int subset = cd_subconjunto(set2, set);
    printf("Es el segundo set subconjunto de el primero?\n");
    assert(subset == 1);
    printf("Test pasado correctamente\n");
    printf("Probemos cambiando el grado de un solo elemento a ver si sigue siedo subconjunto\n");
    int numero = 3;
    Nodo* buscado2 = CD_retornar(set2, &numero);
    CD_modificar_gp(set2, &numero, 1);
    assert(buscado2->gp == 1);
    printf("Ahora el grado de %d se cambio a 1\n", numero);
    
    int subset2 = cd_subconjunto(set2, set);
    printf("Chequeamos ahora si el segundo conjunto es subconjunto del primero. No deberia serlo\n");
    assert(subset2 == 0);
    printf("Test pasado correctamente\n");
    puts("");
    printf("Volvemos a setear el grado como antes para que sea subset\n");
    CD_modificar_gp(set2, &numero, (double)3/11);

    CD difer = CD_diferencia(set, set2);
    puts("");
    imprimir_set_int(difer, imprimir_entero);



    CD_destruir(set);
    CD_destruir(set2);
    CD_destruir(difer);
    return 0;
}















