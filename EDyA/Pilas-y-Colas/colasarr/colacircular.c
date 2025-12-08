#include <stdio.h>
#include <stdlib.h>

#define MAX 10

typedef struct {
    int arr[MAX];
    int primero;
    int ultimo;
    int cantidad; 
} Cola;

Cola* queueCreate() {
    Cola* cola = malloc(sizeof(Cola));
    //cola->arr = malloc(sizeof(int) * MAX);
    cola->primero = 0;
    cola->ultimo = -1;
    cola->cantidad = 0;
    return cola;
}

void queueDestroy(Cola* cola) {
    if (cola) {
       // free(cola->arr);
        free(cola);
    }
}

int cola_es_vacia(Cola* cola) {
    return cola->cantidad == 0;
}

int cola_esta_llena(Cola* cola) {
    return cola->cantidad == MAX;
}

void recorrerCola(Cola* cola) {
    if (cola_es_vacia(cola)) {
        printf("Cola vacía\n");
        return;
    }
    
    int i = cola->primero;
    int contador = 0;
    
    while (contador < cola->cantidad) {
        printf("%d, ", cola->arr[i]);
        i = (i + 1) % MAX;
        contador++;
    }
    printf("\n");
}

void enqueue(Cola* cola, int dato) {
    if (cola_esta_llena(cola)) return; // O manejar error

    cola->ultimo = (cola->ultimo + 1) % MAX;
    cola->arr[cola->ultimo] = dato;
    cola->cantidad++;
}

int dequeue(Cola* cola) {
    if (cola_es_vacia(cola)) {
        printf("Cola vacía\n");
        return -1;  
    }
    
    int dato = cola->arr[cola->primero];
    cola->primero = (cola->primero + 1) % MAX;
    cola->cantidad--;
    
    return dato;
}

int main() {
    Cola* cola = queueCreate();
    enqueue(cola, 1);
    enqueue(cola, 2);
    enqueue(cola, 3);
    enqueue(cola, 4);
    
    printf("Cola: ");
    recorrerCola(cola);
    
    printf("Dequeue: %d\n", dequeue(cola));
    printf("Dequeue: %d\n", dequeue(cola));
    
    printf("Cola después de dequeue: ");
    recorrerCola(cola);
    
    queueDestroy(cola);
    return 0;
}