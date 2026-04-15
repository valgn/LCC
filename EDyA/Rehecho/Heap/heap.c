#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopiadora)(void *dato);
typedef void (*FuncionVisitante)(void *dato);
typedef int (*FuncionComparadora)(void *dato1, void *dato2);


typedef struct {
    void** datos;
    int cant;
    int cap;
    FuncionCopiadora copy;
    FuncionDestructora destr;
    FuncionComparadora comp;
}BinaryHeap;

typedef BinaryHeap* BHeap;

void swap(void** dato1, void** dato2){
    void* temp = *dato1;
    *dato1 = *dato2; *dato2 = temp;
}

BHeap createHeap(int cap, FuncionCopiadora copia, FuncionDestructora destr, FuncionComparadora comp){
    BHeap heap = malloc(sizeof(BinaryHeap));
    heap->cant = 0; heap->cap = cap;
    heap->datos = malloc(sizeof(void*) * cap);
    for(int i = 0; i<cap;i++) heap->datos[i] = NULL;
    heap->copy = copia; heap->destr = destr; heap->comp = comp;
    return heap;
}

void destroyHeap(BHeap heap){
    for(int i = 0; i < heap->cant; i++){
        heap->destr(heap->datos[i]);
    }
    free(heap->datos);
    free(heap);
}

void heapifyUp(BHeap heap){
   int i = heap->cant;
    while(i > 0){
        unsigned padre = (i-1)/2;
        if(heap->comp(heap->datos[padre], heap->datos[i]) < 0){
            swap(&heap->datos[padre], &heap->datos[i]);
            i = padre;
        }else break; 
    }
}

void heapifyDown(BHeap heap, int padre){
    int mayor = padre;
    int leftChild = 2*padre + 1;
    int rightChild = 2*padre + 2;
    if(leftChild < heap->cant && heap->comp(heap->datos[mayor], heap->datos[leftChild]) < 0){
        mayor = leftChild;
    }
    if(rightChild < heap->cant &&heap->comp(heap->datos[mayor], heap->datos[rightChild]) < 0){
        mayor = rightChild;
    }
    if(mayor != padre){
        swap(&heap->datos[mayor], &heap->datos[padre]);
        heapifyDown(heap, mayor);
    }
}


BHeap insertHeap(BHeap heap, void* data){
    if(heap->cant == heap->cap){
        heap->datos = realloc(heap->datos,sizeof(void*) * heap->cap * 2);
        heap->cap = heap->cap * 2;
    } 
    heap->datos[heap->cant] = heap->copy(data);
    heapifyUp(heap);
    heap->cant++;
    return heap;
}

void* removeHeap(BHeap heap){
    if(heap->cant == 0) return NULL;
    void* dato = heap->datos[0];
    heap->datos[0] = heap->datos[heap->cant-1];
    heap->cant--;
    if(heap->cant > 0) heapifyDown(heap, 0);
    return dato;
}

void visitHeap(BHeap heap, FuncionVisitante visit){
    for(int i = 0; i < heap->cant; i++){
        visit(heap->datos[i]);
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

void shiftDown(void** arr, int n, int padre, FuncionComparadora comp){
    int mayor = padre;
    int hijoIzq = padre*2 + 1, hijoDer = padre*2 +2;
    if(hijoIzq < n && comp(arr[mayor], arr[hijoIzq]) < 0) mayor = hijoIzq;
    if(hijoDer < n && comp(arr[mayor], arr[hijoDer]) < 0) mayor = hijoDer;
    if(mayor != padre){
        swap(&arr[mayor], &arr[padre]);
        shiftDown(arr, n, mayor, comp);
    }
}

void heapsort(void** arr, int n, FuncionComparadora comp){
    for(int i = n/2 - 1; i >= 0; i--){
        shiftDown(arr, n, i, comp);
    }
    for(int i = n - 1; i > 0; i--){
        swap(&arr[i], &arr[0]);
        shiftDown(arr, i, 0, comp);
    }
}

int main(){
    void** punteros = malloc(sizeof(void*) * 20);
    for(int i = 20; i > 0; i--){
        int* dato = malloc(sizeof(int));
        *dato = 20 - i;
        punteros[20 - i] = dato;
    }
    heapsort(punteros, 20, comparar_entero);
    for(int i = 0; i < 20; i++){
        imprimir_entero(punteros[i]);
    } 
    puts("");
    for(int i = 0; i<20; i++){
        free(punteros[i]);
    }free(punteros);
    return 0;
}
