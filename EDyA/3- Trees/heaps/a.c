#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int* datos;
    int cantidad;
    int capacidad;
}ColaPrioridad;

void swap(int* i, int* j){
    int t = *i; 
    *i = *j; *j = t; 
}

void flotar(ColaPrioridad* cp, int i){
    // Se inserta al final y se eleva mientras el padre sea mayor
    while(i > 0 && cp->datos[i] > cp->datos[(i-1)/2]){
        swap(&cp->datos[i], &cp->datos[(i-1)/2]);
        i = (i-1)/2;
    }
}

void hundir(ColaPrioridad* cp, int i){
    int padre = i;
    int hijoizq = (2*i) + 1;
    int hijoder = (2*i) + 2;
    int n = cp->capacidad;
    int mayor = padre;
    if(hijoizq < n && cp->datos[hijoizq] > cp->datos[mayor]){
        mayor = hijoizq;
    }
    if(hijoder < n && cp->datos[hijoder] > cp->datos[mayor]){
        mayor = hijoder;
    }
    if(mayor != i){
        swap(&cp->datos[i], &cp->datos[mayor]);
        hundir(cp, mayor);
    }
}

void insertar(ColaPrioridad* cp, int dato){
    if(cp->cantidad == cp->capacidad) return;

    cp->datos[cp->cantidad] = dato;
    flotar(cp, cp->cantidad);
    cp->cantidad++;
}

int extraer(ColaPrioridad* cp){
    if(cp->cantidad == 0) return -1;
    int valor = cp->datos[0];
    cp->datos[0] = cp->datos[cp->cantidad-1];
    cp->cantidad--;
    hundir(cp, 0);
    return valor;
}

void heapsort(ColaPrioridad* cp, int len){
    for(int i = (len/2) - 1; i > 0; i--){
        hundir(cp, i);
    }
    for(int i = len - 1; i > 0; i--){
        swap(&cp->datos[0], &cp->datos[i]);
        hundir(cp, 0);
    }
}