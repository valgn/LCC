#include <stdio.h>
#include <stdlib.h>

typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopiadora)(void *dato);
typedef void (*FuncionVisitante)(void *dato);
typedef int (*FuncionComparadora)(void *dato1, void *dato2);

typedef struct _BHeap{
    void** datos;
    int cap;
    int cantidad;
    FuncionDestructora destroy;
    FuncionComparadora cmp;
    FuncionCopiadora copy;
}BinaryHeap;

typedef BinaryHeap* BHeap;

BHeap bheap_crear(int capacidad, FuncionComparadora cmp, FuncionDestructora destroy, FuncionCopiadora copy){
    BHeap heap = malloc(sizeof(BinaryHeap));
    heap->datos = malloc(sizeof(void*) * capacidad);
    heap->cantidad = 0;
    heap->cantidad = capacidad;
    heap->destroy = destroy; heap->cmp = cmp; heap->copy = copy;
    for(int i = 0; i < capacidad; i ++){
        heap->datos[i] = NULL;
    }
    return heap;
}

void swap(void** dato1, void** dato2){
    void* temp = *dato1;
    *dato1 = *dato2;
    *dato2 = temp;
}

void HeapifyUp(BHeap heap){
    int i = heap->cantidad;
    while(i > 0){
        unsigned padre = (i-1) / 2;
        if(heap->cmp(heap->datos[padre], heap->datos[i]) < 0){
            swap(&heap->datos[padre], &heap->datos[i]);
            i = padre;
        }
        else break;
    }
    return;
}

void HeapifyDown(BHeap heap, int indice){
    int mayor = indice;
    int hijoI = 2 * indice + 1;
    int hijoD = 2 * indice + 2;
    if(hijoI < heap->cantidad && heap->cmp(heap->datos[mayor],heap->datos[hijoI]) < 0){
        mayor = hijoI;
    }
    if(hijoD < heap->cantidad && heap->cmp(heap->datos[mayor],heap->datos[hijoD]) < 0){
        mayor = hijoD;
    }
    if(mayor != indice){
        swap(&heap->datos[mayor], &heap->datos[indice]);
        HeapifyDown(heap, mayor);
    }
    return;
}

BHeap insert_heap(BHeap heap, void* dato){
    if(heap->cantidad == heap->cap){
        heap->datos = realloc(heap->datos, sizeof(void*) * heap->cap * 2);
        heap->cap = heap->cap * 2;
    }
    heap->datos[heap->cantidad] = heap->copy(dato);
    HeapifyUp(heap);
    heap->cantidad++;
    return heap;
}

void* extraer_heap(BHeap heap){
    if(heap->cantidad == 0) return NULL;
    void* ret = heap->datos[0];
    heap->datos[0] = heap->datos[heap->cantidad - 1];
    heap->cantidad--;
    HeapifyDown(heap, 0);
    return ret;
}