#include <stdio.h>

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void HeapifyUp(int* arr, int n){
    while(n > 0){
        int padre = (n-1)/2;
        if(arr[n] < arr[padre]){
            swap(&arr[n], &arr[padre]);
            n = padre;
        }
        else break;
    }
}

void HeapifyDown(int* arr, int len, int i){
    int padre = i;
    int hijoIzq = 2*padre + 1;
    int hijoDer = 2*padre + 2;
    if(hijoIzq < len && arr[hijoIzq] < arr[padre]) padre = hijoIzq;
    if(hijoDer < len && arr[hijoDer] < arr[padre]) padre = hijoDer;

    if(padre != i){
        swap(&arr[padre], &arr[i]);
        HeapifyDown(arr, len, padre);
    }
}

void insertar(int* arr, int* n, int len, int dato){
    if(*n >= len) return;
    arr[*n] = dato;
    HeapifyUp(arr, *n);
    (*n)++;
}

int extraer_min(int* arr, int* n){
    if(*n <= 0) return -1;
    int dato_extraido = arr[0];
    arr[0] = arr[*n-1];
    *n--;
    HeapifyDown(arr, *n, 0);
    return dato_extraido;
}

int main() {
    int heap[100]; // Array estático (o podés hacer malloc de un int*)
    int n = 0;     // Cantidad actual de elementos (size)
    
    insertar(heap, &n, 100, 50);
    insertar(heap, &n, 100, 10);
    insertar(heap, &n, 100, 5);
    insertar(heap, &n, 100, 20);

    // El array ahora es: [5, 20, 10, 50] (Internamente organizado)

    printf("Minimo extraido: %d\n", extraer_min(heap, &n)); // 5
    printf("Minimo extraido: %d\n", extraer_min(heap, &n)); // 10

    return 0;
}