#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include <assert.h>

BTree btree_crear() { return NULL; }

void btree_destruir(BTree nodo, FuncionDestructora destroy)
{
    if (nodo != NULL)
    {

        destroy(nodo->dato);
        btree_destruir(nodo->left, destroy);
        btree_destruir(nodo->right, destroy);
        free(nodo);
    }
}

int btree_empty(BTree nodo) { return nodo == NULL; }

BTree btree_unir(void *dato, BTree left, BTree right, FuncionCopiadora copy)
{
    BTree nuevoNodo = malloc(sizeof(struct _BTNodo));
    assert(nuevoNodo != NULL);
    nuevoNodo->dato = copy(dato);
    nuevoNodo->left = left;
    nuevoNodo->right = right;
    return nuevoNodo;
}

BHeap bheap_crear(int capacidad, FuncionComparadora comp)
{
    BHeap heap = malloc(sizeof(struct _BHeap));
    heap->arr = malloc(sizeof(void *) * (capacidad + 1));
    heap->capacidad = capacidad;
    heap->comp = comp;
    heap->ultimo = 0;
    return heap;
}

void bheap_destruir(BHeap heap, FuncionDestructora destr)
{
    for (int i = 1; i <= heap->ultimo; i++)
    {
        destr(heap->arr[i]);
    }
    free(heap->arr);
    free(heap);
}

int bheap_vacio(BHeap heap)
{
    return heap->ultimo == 0;
}

void heap_recorrer(BHeap heap, FuncionVisitante visit)
{
    for (int i = 1; i <= heap->ultimo; i++)
    {
        visit(heap->arr[i]);
    }
}

BHeap bheap_insertar(BHeap heap, void *dato, FuncionCopiadora copia)
{
    heap->ultimo++;
    int ultimo = heap->ultimo;

    if (heap->ultimo > heap->capacidad)
    {
        heap->capacidad *= 2;
        heap->arr = realloc(heap->arr, sizeof(void *) * (heap->capacidad + 1));
    }

    heap->arr[ultimo] = copia(dato);

    for (int j = ultimo; j > 1 && (heap->comp(heap->arr[j], heap->arr[j / 2]) > 0); j = j / 2)
    {
        void *temp = heap->arr[j];
        heap->arr[j] = heap->arr[j / 2];
        heap->arr[j / 2] = temp;
    }
    return heap;
}

BHeap bheap_eliminar(BHeap heap)
{
    free(heap->arr[1]);
    heap->arr[1] = heap->arr[heap->ultimo];
    heap->ultimo--;
    int j = 1;
    int flag = 1;
    while (2 * j <= heap->ultimo && flag)
    {
        int hijoizq = 2 * j;
        int hijoder = 2 * j + 1;
        int mayor = hijoizq;
        if (hijoder <= heap->ultimo && (heap->comp(heap->arr[hijoder], heap->arr[hijoizq]) > 0))
        {
            mayor = hijoder;
        }
        if (heap->comp(heap->arr[mayor], heap->arr[j]) > 0)
        {
            void *temp = heap->arr[j];
            heap->arr[j] = heap->arr[mayor];
            heap->arr[mayor] = temp;
            j = mayor;
        }
        else
        {
            flag = 0;
        }
    }
    return heap;
}

void *bheap_eliminar_retornador(BHeap heap)
{

    void *maximo = heap->arr[1];

    heap->arr[1] = heap->arr[heap->ultimo];
    heap->ultimo--;
    int j = 1;
    int flag = 1;
    while (2 * j <= heap->ultimo && flag)
    {
        int hijoizq = 2 * j;
        int hijoder = 2 * j + 1;
        int mayor = hijoizq;
        if (hijoder <= heap->ultimo && (heap->comp(heap->arr[hijoder], heap->arr[hijoizq]) > 0))
        {
            mayor = hijoder;
        }
        if (heap->comp(heap->arr[mayor], heap->arr[j]) > 0)
        {
            void *temp = heap->arr[j];
            heap->arr[j] = heap->arr[mayor];
            heap->arr[mayor] = temp;
            j = mayor;
        }
        else
        {
            flag = 0;
        }
    }
    return maximo;
}

BHeap bheap_crear_desde_arr(void **arr, int largo, FuncionCopiadora copiar,
                            FuncionComparadora comp)
{
    BHeap heap = bheap_crear(largo, comp);
    for (int i = 0; i < largo; i++)
    {
        heap->ultimo++;
        if (heap->ultimo > heap->capacidad)
        {
            heap->capacidad = heap->capacidad * 2;
            heap->arr = realloc(heap->arr, sizeof(void *) * (heap->capacidad + 1));
        }
        heap->arr[heap->ultimo] = copiar(arr[i]);

        // Heapify
        for (int j = heap->ultimo; j > 1 && (heap->comp(heap->arr[j], heap->arr[j / 2]) > 0); j = j / 2)
        {
            void *temp = heap->arr[j];
            heap->arr[j] = heap->arr[j / 2];
            heap->arr[j / 2] = temp;
        }
    }
    return heap;
}

int *heapsort(int *arr, int n, FuncionCopiadora copy, FuncionComparadora comp, FuncionDestructora destroy)
{
    void *punteros[n];
    for (int i = 0; i < n; i++)
    {
        punteros[i] = &arr[i];
    }
    BHeap heap = bheap_crear_desde_arr(punteros, n, copy, comp);
    int *nuevoarr = malloc(sizeof(int) * n);

    // printf("%d\n",*(int*)heap->arr[1]);

    for (int i = n - 1; i >= 0; i--)
    {
        void *maximo = bheap_eliminar_retornador(heap);
        nuevoarr[i] = *(int *)maximo;
        // liberar si retornador devuelve copia
        destroy(maximo);
    }

    bheap_destruir(heap, destroy);

    return nuevoarr;
}


static void hundir(void** arr, int n, int i, FuncionComparadora comp) {
    int mayor = i;
    int izq = 2 * i + 1;
    int der = 2 * i + 2;

    // Mientras tenga al menos un hijo a la izquierda
    while (izq < n) {
        mayor = i; // Asumimos que el padre es el mayor

        // Comparamos con hijo izquierdo
        if (comp(arr[izq], arr[mayor]) > 0) {
            mayor = izq;
        }

        // Comparamos con hijo derecho (si existe)
        if (der < n && comp(arr[der], arr[mayor]) > 0) {
            mayor = der;
        }

        // Si el padre ya es el mayor, terminamos (propiedad heap cumplida)
        if (mayor == i) {
            break; 
        }

        // Si no, hacemos SWAP y seguimos bajando
        void* temp = arr[i];
        arr[i] = arr[mayor];
        arr[mayor] = temp;

        // Actualizamos índices para la siguiente vuelta del while
        i = mayor;
        izq = 2 * i + 1;
        der = 2 * i + 2;
    }
}

void heapsort2(void** arr, int len,  FuncionComparadora comp)
{
    // Hundir elementos
    for(int i = len/2 -1; i>=0; i--){
        hundir(arr, len, i , comp);
    }

    for(int i = len-1; i > 0; i--){
        void* temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        hundir(arr, i, 0, comp);
}
}
// cola de prioridad es lo mismo q heap lol xd lol