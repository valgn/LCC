#include <stdio.h>
#include <stdlib.h>

#define MAX 10

typedef struct{
    int* arr;
    int primero;
    int ultimo;

}Cola;


Cola* queueCreate(){
    Cola* cola = malloc(sizeof(Cola));
    cola->arr = malloc(sizeof(int)*MAX);
    cola->primero =-1;
    cola->ultimo = -1;
    return cola;
}

void queueDestroy(Cola* cola){
    free(cola->arr);
    free(cola);
}

int cola_es_vacia(Cola* cola){
    return (cola->primero == -1);
}

int colaLlena(Cola* cola){
    return (cola->ultimo == MAX - 1);
}

void recorrerCola(Cola* cola){
    for(int i = cola->primero; i <= cola->ultimo; i++){
        printf("%d ",cola->arr[i]);
    }
}

int cola_inicio(Cola* cola){
    return cola->arr[cola->primero];
}

void enqueue(Cola** cola, int dato){
    if(colaLlena((*cola))){
        printf("la cola esta llenita\n");
        return;
    }
    if(cola_es_vacia(*cola)){
        (*cola)->primero = 0;
    }
    (*cola)->ultimo++;
    (*cola)->arr[(*cola)->ultimo] = dato;
    
}


int dequeue(Cola* cola){
    if(cola_es_vacia(cola)){
        printf("Chupame las bolas esta vacia la cola vos sabes perro \n");
        return -1;
    }
    int dato = cola->arr[cola->primero];
    if(cola->primero == cola->ultimo){
        cola->primero = -1;
        cola->ultimo = -1;
    }
    else{
        cola->primero++;
    }
    return dato;
}


// (*cola)

int main(){
    Cola* cola = queueCreate();
    enqueue(&cola, 1);
    enqueue(&cola, 2);
    enqueue(&cola, 3);
    enqueue(&cola, 4);
    enqueue(&cola, 4);
    enqueue(&cola, 10);
    enqueue(&cola, 10);
    enqueue(&cola, 10);
    enqueue(&cola, 10);
    enqueue(&cola, 10);
    enqueue(&cola, 10);

    //printf("%d", cola_es_vacia(cola));
    printf("La cola actualemente es: \n");
    recorrerCola(cola);
    printf("\n");
    int i = 0;
    for(; i < 7; i++){
        printf("Saque el dato: %d", dequeue(cola));
        printf("\n");
    }
    printf("La cola tras sacar los primeros %d elementos quedo: \n", i);
    recorrerCola(cola);
    printf("\n");


    return 0;
}