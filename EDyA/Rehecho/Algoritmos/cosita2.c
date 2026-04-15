#include <stdio.h>

// 1. Clásica: Array ordenado. Retorna índice o -1.
int busqueda_binaria(int* arr, int n, int target);

// 2. Montaña: Array que sube y baja. Retorna el VALOR máximo.
// Precondición: El array aumenta hasta un pico y luego disminuye.
int buscar_pico(int* arr, int n);

// 3. Raíz: Retorna floor(sqrt(x)).
// Ej: raiz(20) -> 4.
int raiz_cuadrada(int x);

int busqueda_binaria(int* arr, int n, int target){
    int left = 0; int final = n-1;

    while(left <= final){
        int medio = (final+left)/2;
        if(arr[medio] > target) final = medio - 1;
        else if(arr[medio] < target) left = medio + 1;
        else return medio;
    }
    return -1;
}


int buscar_pico(int* arr, int n){
    int i = 0; int fin = n-1;
    while(i <= fin){
        int medio = ( fin + i ) / 2;
        if(arr[medio] < arr[medio+1]) i = medio + 1;
        else if(arr[medio] > arr[medio+1]) fin = medio - 1;
        else return arr[medio];
    }
    return arr[i];
}

int raiz_cuadrada(int x){
    int izq = 0; int der = x/2;
    while(izq <= der){
        int medio = (der+izq)/2;
        if(medio*medio < x) izq = medio+1;
        else if(medio * medio > x) der = medio-1;
        else return medio;
    }
}

int main() {
    // TEST 1
    int arr1[] = {1, 3, 5, 7, 9, 11, 15, 20};
    printf("Busqueda 7 (idx 3): %d\n", busqueda_binaria(arr1, 8, 7));
    printf("Busqueda 4 (idx -1): %d\n", busqueda_binaria(arr1, 8, 4));

    // TEST 2
    int montana[] = {1, 3, 8, 12, 4, 2}; // El pico es 12
    printf("Pico (12): %d\n", buscar_pico(montana, 6));
    
    int montana2[] = {10, 20, 30, 40, 50}; // Pico en el borde
    printf("Pico (50): %d\n", buscar_pico(montana2, 5));

    // TEST 3
    printf("Raiz 16 (4): %d\n", raiz_cuadrada(16));
    printf("Raiz 20 (4): %d\n", raiz_cuadrada(20));
    printf("Raiz 100 (10): %d\n", raiz_cuadrada(100));

    return 0;
}