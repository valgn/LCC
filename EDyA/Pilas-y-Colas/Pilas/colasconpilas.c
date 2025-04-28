#include <stdio.h>
#include <stdlib.h>
#include "pilas.h"
#define CAP 100
//cola_crear,cola_destruir, cola_vacia, enqueue, dqueue, cola imprimir

Cola cola_crear(){
    Cola cola;
    cola.p1= pila_crear(CAP);
    cola.p2= pila_crear(CAP);
    return cola;
}

void cola_destruir(Cola cola){
    pila_destruir(cola.p1);
    pila_destruir(cola.p2);
}

int cola_vacia(Cola cola){
    return pila_es_vacia(cola.p1)&& pila_es_vacia(cola.p2);
}

Cola enqueue(Cola cola , int dato){
    pila_apilar(cola.p1, dato);

    return cola;
}

int dqueue(Cola cola){
    if (pila_es_vacia(cola.p2)){
        for(;!pila_es_vacia(cola.p1);){
            pila_apilar(cola.p2, pila_desapilar2(cola.p1));
        }
    }
    
    int retorno = pila_desapilar2(cola.p2);
    return retorno;
}

void cola_imprimir(Cola cola){
    
        for(;!pila_es_vacia(cola.p2);){
            pila_apilar(cola.p1, pila_desapilar2(cola.p2));
             }
    for(int i = 0;i<=cola.p1->ultimo;i++){
        printf("%d ",cola.p1->arr->direccion[i]);
    }
}


int main() {
    Cola c = cola_crear();

    // Encolar elementos
    c = enqueue(c, 10);
    c = enqueue(c, 20);
    c = enqueue(c, 2);
    c = enqueue(c, 1);
    c = enqueue(c, 3);
    c = enqueue(c, 5);
    
    cola_imprimir(c);
    
    int a = dqueue(c);

    
    printf("\n");

    // Desencolar un elemento
    printf("Elemento desencolado: %d\n", a);

    printf("Cola después de desencolar:\n");
    cola_imprimir(c);
    printf("\n");

    // Liberar recursos
    cola_destruir(c);
    return 0;
}
