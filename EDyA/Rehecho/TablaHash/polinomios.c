#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _GNode {
  double data;
  int exp;
  struct _GNode *next;
} GNode;

typedef struct _GNode *GList;

/**
 * Devuelve una lista vacía.
 */
GList glist_crear() { 
  return NULL; 
}

/**
 * Destruccion de la lista.
 */
void glist_destruir(GList list) {
  GNode *nodeToDelete;
  while (list != NULL) {
    nodeToDelete = list;
    list = list->next;
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
GList glist_agregar_inicio(GList list, double data, int exp) {
  GNode *newNode = malloc(sizeof(GNode));
  assert(newNode != NULL);
  newNode->next = list;
  newNode->data = data;
  newNode->exp = exp;
  return newNode;
}

/**
 * Busca un elemento en la lista.
 */
int glist_buscar(GList list, int exp){
  int found = 0;
  for(GNode *node = list; found == 0 && node != NULL; node = node->next)
    if ((node->exp - exp) == 0) found = 1;
  return found;
}

GNode* glist_buscar_nodo(GList list, int exp){
  GNode* found = NULL;
  for(GNode *node = list; found == 0 && node != NULL; node = node->next)
    if ((node->exp - exp) == 0) found = node;
  return found;
}

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void glist_recorrer(GList list) {
  for (GNode *node = list; node != NULL; node = node->next){
    if(node->exp == 0) printf("%.1f ", node->data);
    else printf("%.1fx^%d ", node->data, node->exp);
  }

}

GList glist_remover(GList lista, int exp){
    GNode* temp = lista;
    GNode* ant = NULL;
    while(temp != NULL && (temp->exp-exp) != 0){
        ant = temp;
        temp = temp->next;
    }
    if(temp != NULL) return lista;
    if(ant == NULL) lista = lista->next;
    else{
        ant->next = temp->next;
    }
    free(temp);
    return lista;
}


/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
    GList head;
} CasillaHash;

/**
 * Estructura principal que representa la tabla hash.
 */
struct _TablaHash {
    CasillaHash *elems;
    unsigned numElems;
    unsigned capacidad;
};
typedef struct _TablaHash *TablaHash;

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad) {

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(CasillaHash) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx].head = NULL;
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
    if (tabla->elems[idx].head != NULL)
      glist_destruir(tabla->elems[idx].head);

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}




/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */
void tablahash_insertar(TablaHash tabla, double dato, int exp) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = exp % tabla->capacidad;
  GNode* buscado = glist_buscar_nodo(tabla->elems[idx].head, exp);
  if(buscado){// Sobreescribir
    buscado->data = dato;
    buscado->exp = exp;
    return;
  }
  else{
    tabla->elems[idx].head = glist_agregar_inicio(tabla->elems[idx].head, dato, exp);
    tabla->numElems++;
  }
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
GNode* tablahash_buscar(TablaHash tabla, int exp) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = exp % tabla->capacidad;
  GNode* buscado = glist_buscar_nodo(tabla->elems[idx].head, exp);
  if(buscado) return buscado;
  else return NULL;
}

void printearPolinomio(TablaHash p){
    for(int i = 0; i < p->capacidad; i++){
        if(p->elems[i].head != NULL)
        glist_recorrer(p->elems[i].head);
    }
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, int exp) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = exp % tabla->capacidad;
  if(tabla->elems[idx].head == NULL) return;
  else{
    tabla->elems[idx].head = glist_remover(tabla->elems[idx].head, exp);
  }
}

TablaHash suma_polinomios(TablaHash P1, TablaHash P2){
    TablaHash res = tablahash_crear(P1->capacidad + P2->capacidad);
    for(int i = 0; i < P1->capacidad; i++){
        if(P1->elems[i].head != NULL){
            GNode* temp = P1->elems[i].head;
            while(temp != NULL){
                tablahash_insertar(res, temp->data, temp->exp);
                temp = temp->next;
            }
        }
    }
    for(int i = 0; i < P2->capacidad; i++){
        if(P2->elems[i].head != NULL){
            GNode* temp = P2->elems[i].head;
            while(temp != NULL){
                GNode* buscado = tablahash_buscar(res, temp->exp);
                if(buscado == NULL){
                    tablahash_insertar(res, temp->data, temp->exp);
                }
                else{
                    buscado->data = buscado->data + temp->data;
                    if(buscado->data == 0) tablahash_eliminar(res, buscado->exp);
                }
                temp = temp->next;
            }
        }
    }
    return res;
}

int main(){
    TablaHash p1 = tablahash_crear(3);
    TablaHash p2 = tablahash_crear(2);
    double coef[3] = {2,4,6};
    double coef2[3] = {3,1,1};
    int exp[3] = {0, 1, 2};
    int exp2[3] = {1,2,3};
    for(int i = 0; i < 3; i++) tablahash_insertar(p1, coef[i], exp[i]);
    for(int i = 0; i < 3; i++) tablahash_insertar(p2, coef2[i], exp2[i]);
    printearPolinomio(p1);
    puts("");
    printearPolinomio(p2);
    TablaHash suma = suma_polinomios(p1, p2);
    puts("");
    printearPolinomio(suma);
    return 0;
}