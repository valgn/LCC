#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopia)(void *dato);
typedef void (*FuncionVisitante)(void *dato);
typedef int (*FuncionComparar)(void *dato1, void *dato2);
typedef int (*Predicado) (void *dato);

typedef struct _GNode {
  void *data;
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

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void glist_recorrer(GList list, FuncionVisitante visit) {
  for (GNode *node = list; node != NULL; node = node->next)
    visit(node->data);
}

void split(GList fuente, GList* first, GList* second){
    if(fuente == NULL || fuente->next == NULL) return;
    GNode* lento = fuente, *rapido = fuente->next;
    while(rapido != NULL && rapido->next != NULL){
        lento = lento->next;
        rapido = rapido->next->next;
    }
    *first = fuente;
    *second = lento->next;
    lento->next = NULL;
}

GList merge(GList a, GList b, FuncionComparar cmp){
    if(a == NULL) return b;
    if(b == NULL) return a;
    GList res = NULL;
    if(cmp(a->data, b->data) < 0){
        res = a;
        a = a->next;
    }
    else if(cmp(b->data, a->data) < 0){
        res = b;
        b = b->next;
    }
    res->next = merge(a, b, cmp);
    return res;
}

void mergesort(GList* lista, FuncionComparar cmp){
    GList cabeza = *lista;
    GList a, b = NULL;
    if(cabeza == NULL || cabeza->next == NULL) return;
    split(cabeza, &a, &b);
    mergesort(&a, cmp);
    mergesort(&b, cmp);

    *lista = merge(a, b, cmp);
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

int main(){
    GList lista = glist_crear();
    
    for(int i = 0; i < 20; i++){
        lista = glist_agregar_inicio(lista, &i, copiar_entero);
    }
    mergesort(&lista, comparar_entero);
    glist_recorrer(lista, imprimir_entero);


    return 0;
}
