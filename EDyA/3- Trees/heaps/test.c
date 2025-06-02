#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

static void imprimir_puntero_entero(void* i) {
  printf("%d ", *(int*)i);
}

static void* copiar_puntero_entero(void* i) {
  int* p = malloc(sizeof(int));
  *p = *(int*)i;
  return p;
}

static void destruir_puntero_entero(void* i) { free(i); }


static int comparar_puntero_entero(void* i1, void* i2) {
  return (*(int*)i1 - *(int*)i2);
}

int main() {
    BHeap heap = bheap_crear(8, comparar_puntero_entero);

    int a[] = {10, 20, 15, 25, 30, 16, 18, 19};
    int n = sizeof(a) / sizeof(a[0]);

    for (int i = 0; i < n; i++) {
        heap = bheap_insertar(heap, &a[i], copiar_puntero_entero);
    }   
    heap = bheap_eliminar(heap);  // elimina el 30
    heap_recorrer(heap, imprimir_puntero_entero);
    puts("");

    bheap_destruir(heap, destruir_puntero_entero);

    return 0;
}