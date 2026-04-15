#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopia)(void *dato);
typedef void (*FuncionVisitante)(void *dato);
typedef int (*FuncionComparar)(void *dato1, void *dato2);
typedef int (*Predicado) (void *dato);

typedef unsigned (*FuncionHasheadora)(void*);

typedef struct _GNode {
  void *data;
  struct _GNode *next;
} GNode;

typedef struct _GNode* GList;


/**
 * Devuelve una lista vacía.
 */
GList glist_crear() { 
  return NULL; 
}

/**
 * Destruccion de la lista.
 */
void glist_destruir(GList list, FuncionDestructora destroy) {
  GNode *nodeToDelete;
  while (list != NULL) {
    nodeToDelete = list;
    list = list->next;
    destroy(nodeToDelete->data); 
    free(nodeToDelete);
  }
}

/**
 * Determina si la lista es vacía.
 */
int glist_vacia(GList list) { 
  return (list == NULL); 
}

/**
 * Agrega un elemento al inicio de la lista.
 */
GList glist_agregar_inicio(GList list, void *data, FuncionCopia copy) {
  GNode *newNode = malloc(sizeof(GNode));
  assert(newNode != NULL);
  newNode->next = list;
  newNode->data = copy(data);
  return newNode;
}

/**
 * Busca un elemento en la lista.
 */
int glist_buscar(GList list, void* data, FuncionComparar comp){
  int found = 0;
  for(GNode *node = list; found == 0 && node != NULL; node = node->next)
    if (comp(node->data, data) == 0) found = 1;
  return found;
}

GNode* nodo_buscar(GList list, void* dato, FuncionComparar comp){
    GNode* found = list;
    while(found != NULL && comp(dato, found->data) != 0){
        found = found->next;
    }
    return found;
}

void* dato_buscar(GList list, void* dato, FuncionComparar comp){
    void* found = NULL;
  for(GNode *node = list; found == NULL && node != NULL; node = node->next)
    if (comp(node->data, dato) == 0) found = node->data;
  return found;
}

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void glist_recorrer(GList list, FuncionVisitante visit) {
  for (GNode *node = list; node != NULL; node = node->next)
    visit(node->data);
}

GList glistRemove(GList list, void* dato, FuncionComparar comp, FuncionDestructora destroy){
    GNode* actual = list;
    GNode* ant = NULL;
    while(actual != NULL && comp(dato, actual->data) != 0){
        ant = actual;
        actual = actual->next;
    }
    if(actual == NULL) return list;
    else if(ant == NULL){
        list = list->next;
    }
    else{
        ant->next = actual->next;
    }
    destroy(actual->data);
    free(actual);
    return list;
}

int glistLength(GList list){
    GNode* visitor = list;
    int count = 0;
    while(visitor != NULL){
        count++;
        visitor = visitor->next;
    }
    return count;
}

/**
 * Estructura principal que representa la tabla hash.
 */
struct _TablaHash {
  GList *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionCopia copia;
  FuncionComparar comp;
  FuncionDestructora destr;
  FuncionHasheadora hash;
};
typedef struct _TablaHash* TablaHash;

TablaHash createHashTable(int capacidad, FuncionCopia copy, FuncionComparar comp, FuncionDestructora destroy, 
    FuncionHasheadora hash){
        TablaHash tabla = malloc(sizeof(struct _TablaHash));
        tabla->elems = malloc(sizeof(GList) * capacidad);
        tabla->capacidad = capacidad;
        tabla->numElems = 0;
        tabla->comp = comp; tabla->copia = copy; tabla->destr = destroy; tabla->hash = hash;
        for(int i = 0; i < capacidad; i++){
            tabla->elems[i] = NULL;
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

void tablahash_destruir(TablaHash tabla) {

  // Destruir cada uno de los datos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx){
    GList temp = tabla->elems[idx];
    glist_destruir(temp, tabla->destr);
  }
  free(tabla->elems);
  free(tabla);
  return;
}

void rehashear(TablaHash tabla){
    unsigned int newCap = tabla->capacidad * 2;
    GList* newElems = malloc(sizeof(GList) * newCap);
    for(int i = 0; i < newCap; i++){
        newElems[i] = NULL;
    }
    for(int i = 0; i<tabla->capacidad; i++){
        if(tabla->elems[i] != NULL){
            GNode* temp = tabla->elems[i];
            while(temp != NULL){
                unsigned newIdx = tabla->hash(temp->data) % newCap;
                GNode* saved = temp->next;
                temp->next = newElems[newIdx];
                newElems[newIdx] = temp;
                temp = saved;
            }
        }
    }
    free(tabla->elems);
    tabla->elems = newElems;
    tabla->capacidad = newCap;
}

void tablahash_insertar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  if(glistLength(tabla->elems[idx]) >= 5){
    rehashear(tabla);
    idx = tabla->hash(dato) % tabla->capacidad;
  }

  // Insertar el dato si la casilla estaba libre.
  if (tabla->elems[idx] == NULL) {
    tabla->numElems++;
    tabla->elems[idx] = glist_agregar_inicio(tabla->elems[idx], dato, tabla->copia);
    return;
  }
  // Sobrescribir el dato si el mismo ya se encontraba en la tabla.
  GNode* found = nodo_buscar(tabla->elems[idx], dato, tabla->comp);
  if (found) {
    tabla->destr(found->data);
    found->data = tabla->copia(dato);
    return;
  }
  else {
    tabla->elems[idx] = glist_agregar_inicio(tabla->elems[idx], dato, tabla->copia);
    tabla->numElems++;
    return;
  }
}


void *tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Retornar NULL si la casilla estaba vacia.
  if (tabla->elems[idx] == NULL)
    return NULL;
  // Retornar el dato de la casilla si hay concidencia.
  return dato_buscar(tabla->elems[idx], dato, tabla->comp); // Si no esta en la lista retorna null, sino devuelve el dato
}

void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Retornar si la casilla estaba vacia.
  if (tabla->elems[idx] == NULL)
    return;
  // Vaciar la casilla si hay coincidencia.
  else if (glist_buscar(tabla->elems[idx], dato, tabla->comp)) {
    tabla->elems[idx] = glistRemove(tabla->elems[idx], dato, tabla->comp, tabla->destr);    
    tabla->numElems--;
    return;
  }
}



void imprimir_hash(TablaHash tabla, FuncionVisitante visit){
    for(int i = 0; i < tabla->capacidad; i++){
        printf("La casilla %d tiene la lista conformada por: ", i);
        glist_recorrer(tabla->elems[i], visit);
        puts("");
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
    TablaHash tabla = createHashTable(5, copiar_entero, comparar_entero, destruir_entero, hashear_entero);
    for(int i = 0; i < 50; i++){
        tablahash_insertar(tabla, &i);
    }
    imprimir_hash(tabla, imprimir_entero);
    printf("La tabla tiene %d elementos y capacidad %d.\n", tabla->numElems, tabla->capacidad);
    puts("");

    int eliminar[4] = {15,11,7,3};
    for(int i = 0; i < 4; i++){
        tablahash_eliminar(tabla, &eliminar[i]);
    }
    printf("La tabla despues de eliminar:\n");
    imprimir_hash(tabla, imprimir_entero);
    printf("La tabla tiene %d elementos.\n", tabla->numElems);
    puts("");
    printf("La tabla tiene exactamente 5 elementos en cada casilla antes de eliminar, si meto 1 deberia rehashear.\n");
    for(int i = 0; i < 4; i++){
        tablahash_insertar(tabla, &eliminar[i]);
    }
    //Meto el primer elemento de eliminar y printeo la cantidad
    int fifty = 5;
    tablahash_insertar(tabla, &fifty);
    printf("La tabla hash tiene capacidad %d con %d elementos.\n", tabla->capacidad, tabla->numElems);
    imprimir_hash(tabla, imprimir_entero);
    puts("");



    tablahash_destruir(tabla);
    return 0;
}