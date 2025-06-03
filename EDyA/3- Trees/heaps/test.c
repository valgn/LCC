#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include <time.h>

// static void imprimir_puntero_entero(void* i) {
//   printf("%d ", *(int*)i);
// }

static void* copiar_puntero_entero(void* i) {
  int* p = malloc(sizeof(int));
  *p = *(int*)i;
  return p;
}

// static void destruir_puntero_entero(void* i) { free(i); }


static int comparar_puntero_entero(void* i1, void* i2) {
  return (*(int*)i1 - *(int*)i2);
}

int main() {
    //BHeap heap = bheap_crear(8, comparar_puntero_entero);

    srand (time(NULL));


    int* a = malloc(sizeof(int)*10000);
    //int n = sizeof(a) / sizeof(a[0]);

    for(int i = 0; i<10000; i++){
      int num = rand() % 10001;
      a[i] = num;
      }
    // for (int i = 0; i < n; i++) {
    //     heap = bheap_insertar(heap, &a[i], copiar_puntero_entero);
    // }   
    // heap = bheap_eliminar(heap);  // elimina el 30
    // void* punteros[n];
    // for (int i = 0; i < n; i++) {
    //     punteros[i] = &a[i];
    // }


    //BHeap heaparr = bheap_crear_desde_arr(punteros, n, (FuncionCopiadora)copiar_puntero_entero, 
    //(FuncionComparadora)comparar_puntero_entero);

    //heap_recorrer(heaparr, imprimir_puntero_entero);
    puts("");

    //bheap_destruir(heaparr, destruir_puntero_entero);

    int* arrordenado = heapsort(a, 10000 , (FuncionCopiadora)copiar_puntero_entero, 
    (FuncionComparadora)comparar_puntero_entero);

    for(int i = 0; i<10000;i++){
      printf("%d ", arrordenado[i]);
    }
    //printf("%d\n",arrordenado[0]);

    

    return 0;
}