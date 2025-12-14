#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


typedef void *(*FuncionCopiadora)(void *dato);
/** Retorna una copia fisica del dato */
typedef int (*FuncionComparadora)(void *dato1, void *dato2);
/** Retorna un entero negativo si dato1 < dato2, 0 si son iguales y un entero
 * positivo si dato1 > dato2  */
typedef void (*FuncionDestructora)(void *dato);
/** Libera la memoria alocada para el dato */
typedef unsigned (*FuncionHash)(void *dato);
/** Retorna un entero sin signo para el dato */

typedef struct _TNodo{
    void* dato;
    double gp;
    struct _TNodo* sig;
}Tnodo;


typedef struct {
  Tnodo* head;
} CasillaHash;

/**
 * Estructura principal que representa la tabla hash.
 */
struct _CD {
  CasillaHash *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

typedef struct _CD* CD;

CD cd_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash) {

  // Pedimos memoria para la estructura principal y las casillas.
  CD tabla = malloc(sizeof(struct _CD));
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
    tabla->elems[idx].head = NULL;
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
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx)
    if (tabla->elems[idx].head != NULL){
        Tnodo* actual = tabla->elems[idx].head;
        while(actual != NULL){
            Tnodo* temp = actual->sig;
            tabla->destr(actual->dato);
            free(actual);
            actual = temp;
        }
    }

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}


void slist_destruir(Tnodo* lista) {
  Tnodo *nodoAEliminar;
  while (lista != NULL) {
    nodoAEliminar = lista;
    lista = lista->sig;
    free(nodoAEliminar);
  }
}

int slist_vacia(Tnodo *lista) {
  return lista == NULL;
}

Tnodo* slist_agregar_inicio(Tnodo* lista, void* dato, double grado,FuncionCopiadora copy) {
  Tnodo *nuevoNodo = malloc(sizeof(Tnodo));
  nuevoNodo->dato = copy(dato);
  nuevoNodo->gp = grado;
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

int slist_buscar(Tnodo* lista, void* dato, FuncionComparadora comp){
    Tnodo* temp = lista;
    while(temp != NULL && comp(temp->dato, dato)){
        temp = temp->sig;
    }
    if(temp == NULL) return 0;
    return 1;
}

Tnodo* slist_retornar(Tnodo* lista, void* dato, FuncionComparadora comp){
    Tnodo* temp = lista;
    while(temp != NULL && comp(temp->dato, dato)){
        temp = temp->sig;
    }
    if(temp == NULL) return NULL;
    return temp;
}


/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */
void cd_agregar(CD tabla, void *dato, double grado) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  Tnodo* cambiar = slist_retornar(tabla->elems[idx].head, dato, tabla->comp);
  // Insertar el dato si la casilla estaba libre.
  if (tabla->elems[idx].head == NULL) {
    tabla->numElems++;
    tabla->elems[idx].head = slist_agregar_inicio(tabla->elems[idx].head, dato,grado ,tabla->copia);
    return;
  }
  // Si el dato ya esta en la tabla no hacer nada
  else if (cambiar) {
    cambiar->gp = grado;
    return;
  }
  else {
    tabla->numElems++;
    tabla->elems[idx].head = slist_agregar_inicio(tabla->elems[idx].head, dato, grado,tabla->copia);
    return;
  }
}



/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *CD_buscar(CD tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  Tnodo* buscado = slist_retornar(tabla->elems[idx].head, dato, tabla->comp);
  // Retornar NULL si la casilla estaba vacia.
  if (tabla->elems[idx].head == NULL){
    return NULL;
  }
  else if (buscado)
    return buscado->dato;
  // Retornar NULL en otro caso.
  else
    return NULL;
}

Tnodo* CD_retornar(CD tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  Tnodo* buscado = slist_retornar(tabla->elems[idx].head, dato, tabla->comp);
  // Retornar NULL si la casilla estaba vacia.
  if (tabla->elems[idx].head == NULL){
    return NULL;
  }
  else if (buscado)
    return buscado;
  // Retornar NULL en otro caso.
  else
    return NULL;
}



/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void CD_eliminar(CD tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Retornar si la casilla estaba vacia.
  if (tabla->elems[idx].head == NULL)
    return;
  // Vaciar la casilla si hay coincidencia.
  else if (tabla->comp(tabla->elems[idx].head->dato, dato) == 0) {
    tabla->numElems--;
    tabla->destr(tabla->elems[idx].head->dato);
    Tnodo* temp = tabla->elems[idx].head;
    tabla->elems[idx].head = tabla->elems[idx].head->sig;
    free(temp);
    return;
  }
  else{
    Tnodo* temp = tabla->elems[idx].head;
    while(temp->sig != NULL && tabla->comp(temp->sig->dato, dato)) temp = temp->sig;
    if(temp->sig == NULL) return;
    Tnodo* sig = temp->sig->sig;
    tabla->destr(temp->sig->dato);
    free(temp->sig);
    temp->sig = sig;
    return;
  }
}

void cd_modificar_gp(CD set, void* dato, double gp){
    unsigned idx = set->hash(dato) % set->capacidad;
    Tnodo* cambiar = slist_retornar(set->elems[idx].head, dato, set->comp);
    if(!cambiar) return;
    else{
        if(gp == 0){
        CD_eliminar(set, dato);
        }
        else{
            cambiar->gp = gp;
        }
    }
}


int cd_subconjunto(CD A, CD B){
    for(int i = 0; i < A->capacidad; i++){
        if(A->elems[i].head != NULL){
            Tnodo* temp = A->elems[i].head;
            while(temp != NULL){
                Tnodo* bitem = CD_retornar(B, temp->dato);
                if(!bitem) return 0;
                else if(bitem->gp < temp->gp) return 0;
                temp = temp->sig;
            }
        }
    }
    return 1;
}


void cd_agregar_max(CD tabla, void *dato, double grado) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  Tnodo* cambiar = slist_retornar(tabla->elems[idx].head, dato, tabla->comp);
  // Insertar el dato si la casilla estaba libre.
  if (tabla->elems[idx].head == NULL) {
    tabla->numElems++;
    tabla->elems[idx].head = slist_agregar_inicio(tabla->elems[idx].head, dato,grado ,tabla->copia);
    return;
  }
  // Si el dato ya esta en la tabla, cambiarle el grado
  else if (cambiar) {
    if(grado > cambiar->gp){
        cambiar->gp = grado;
    }
    return;
  }
  else {
    tabla->numElems++;
    tabla->elems[idx].head = slist_agregar_inicio(tabla->elems[idx].head, dato, grado,tabla->copia);
    return;
  }
}

CD cd_union(CD A, CD B){
    CD res = cd_crear(A->capacidad + B->capacidad, B->copia, B->comp, B->destr, B->hash);
    for(int i = 0; i < A->capacidad; i++){
        if(A->elems[i].head != NULL){
            Tnodo* temp = A->elems[i].head;
            while(temp != NULL){
                cd_agregar(res, temp->dato, temp->gp);
                temp = temp->sig;
            }
        }
    }
    for(int i = 0; i < B->capacidad; i++){
        if(B->elems[i].head != NULL){
            Tnodo* temp = B->elems[i].head;
            while(temp != NULL){
                cd_agregar_max(res, temp->dato, temp->gp);
                temp = temp->sig;
            }
        }
    }
    return res;
}

CD diferencia(CD a, CD b){
    CD res = cd_crear(a->capacidad, a->copia, a->comp, a->destr, a->hash);
    for(int i = 0; i < a->capacidad; i++){
        if(a->elems[i].head != NULL){
            Tnodo* temp = a->elems[i].head;
            while(temp != NULL){
                Tnodo* buscado = CD_retornar(b, temp->dato);
                if(buscado){
                    if(temp->gp < buscado->gp){
                        cd_agregar(res, temp->dato, 0);
                    }
                    else cd_agregar(res, temp->dato, temp->gp-buscado->gp);
                }
                else{
                    cd_agregar(res, temp->dato, temp->gp);
                }
                temp = temp->sig;
            }
        }
    }
    return res;
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
        if(set->elems[i].head != NULL){
            Tnodo* temp = set->elems[i].head;
            while(temp != NULL){
                imprimir(temp->dato);
                temp = temp->sig;
            }
        }
    }
}


int main(){
    CD set = cd_crear(10, copiar_entero, comparar_enteros, destruir_entero, hashear_entero);
    for(int i = 0; i < set->capacidad; i++){
        cd_agregar(set, &i, (double)i/set->capacidad);
    }
    // imprimir_set_int(set, imprimir_entero);
    // Veamos el grado de el numero 9 insertado. Tendria que ser 0.9
    int valor_buscado = 9;
    printf("BUSCAMOS EL 9 Y VEMOS SU GRADO.\n");
    Tnodo* buscado = CD_retornar(set, &valor_buscado);
    printf("Testeando...\n");
    assert(buscado->gp == 0.9);
    printf("Test pasado correctamente.\n");
    puts("");
    printf("Cambiemos el grado del 9 a otro valor, 0.5\n");
    cd_modificar_gp(set, &valor_buscado, 0.5);
    printf("Testeando si el grado es 0.5...\n");
    assert(buscado->gp == 0.5);
    printf("Test pasado correctamente.\n");

    puts("");

    CD set2 = cd_crear(5, copiar_entero, comparar_enteros, destruir_entero, hashear_entero);
    for(int i = 2; i < set2->capacidad+2; i++){
        cd_agregar(set2, &i, (double)i/11);
    }
    int subset = cd_subconjunto(set2, set);
    printf("Es el segundo set subconjunto de el primero?\n");
    assert(subset == 1);
    printf("Test pasado correctamente\n");
    printf("Probemos cambiando el grado de un solo elemento a ver si sigue siedo subconjunto\n");
    int numero = 3;
    Tnodo* buscado2 = CD_retornar(set2, &numero);
    cd_modificar_gp(set2, &numero, 1);
    assert(buscado2->gp == 1);
    printf("Ahora el grado de %d se cambio a 1\n", numero);
    
    int subset2 = cd_subconjunto(set2, set);
    printf("Chequeamos ahora si el segundo conjunto es subconjunto del primero. No deberia serlo\n");
    assert(subset2 == 0);
    printf("Test pasado correctamente\n");
    puts("");
    printf("Volvemos a setear el grado como antes para que sea subset\n");
    cd_modificar_gp(set2, &numero, (double)3/11);

    CD difer = diferencia(set, set2);
    puts("");
    imprimir_set_int(difer, imprimir_entero);



    CD_destruir(set);


    return 0;
}