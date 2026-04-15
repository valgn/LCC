#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef void (*FuncionDestructora)(void*);
typedef int (*FuncionPredicado)(void*);
typedef void* (*FuncionCopiadora)(void*);
typedef void (*FuncionVisitante)(void*);
typedef int (*FuncionComparadora)(void*, void*);

typedef struct _GNode {
  void *data;
  struct _GNode *next;
} GNode;

typedef GNode* GList;

GList merge(GList a, GList b, FuncionComparadora cmp){
    if(a == NULL) return b;
    if(b == NULL) return a;
    GList res = NULL;
    if(cmp(a->data, b->data) < 0){
        res = a;
        a = a->next;
    }
    else if(cmp(a->data, b->data) > 0){
        res = b;
        b = b->next;
    }
    res->next = merge(a, b, cmp);
    return res;
}

void split(GList fuente, GList* frente, GList* atras){
    if(fuente == NULL || fuente->next == NULL) return;
    GNode* lento = fuente; GNode* rapido = fuente->next;
    while(rapido != NULL && rapido->next != NULL){
        lento = lento->next;
        rapido = rapido->next->next;
    }
    *frente = fuente;
    *atras = lento->next;
    lento->next = NULL;
}

void mergesort(GList* cabeza, FuncionComparadora cmp){
    GList fuente = *cabeza;
    GList a, b = NULL;
    if(fuente == NULL || fuente->next == NULL) return;
    split(fuente, &a, &b);
    mergesort(&a, cmp); mergesort(&b, cmp);

    *cabeza = merge(a, b, cmp);
}

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
GList glist_agregar_inicio(GList list, void *data, FuncionCopiadora copy) {
  GNode *newNode = malloc(sizeof(GNode));
  assert(newNode != NULL);
  newNode->next = list;
  newNode->data = copy(data);
  return newNode;
}

/**
 * Busca un elemento en la lista.
 */
int glist_buscar(GList list, void* data, FuncionComparadora comp){
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
