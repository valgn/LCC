#include "listas.h"
#include <stdio.h>
#include <stdlib.h>

#define CAPACIDAD 10

Lista crear_lista(){
    Lista lista;
    lista.elem = malloc(sizeof(int) * CAPACIDAD);
    lista.cant = 0;
    lista.cap = CAPACIDAD;

    if(lista.elem == NULL){
        printf("El malloc fallo.\n");
    }
    return lista;
}

void destruir_lista(Lista* lista){
    free(lista->elem);

}

void agregar_lista(Lista* lista, int elemento, int final){ //Final para 0d
    if(lista->cant >= lista->cap){
        lista->cap = lista->cap * 2;
        lista->elem = realloc(lista->elem, lista->cap * sizeof(int));
        if(lista->elem == NULL){
            printf("Fallo el realloc.\n");
        }
    }
    if(final){
        lista->elem[lista->cant++] = elemento;
    }
    else{
        //Shift de elementos para meter el 0 a la izq
        for(int i = lista->cant; i > 0; i--){
            lista->elem[i] = lista->elem[i - 1];
        }
        lista->elem[0] = 0;
        lista->cant++;
    }
}

void sucesor_lista(Lista* lista, int final){
    if(lista->cant == 0){
        printf("La lista está vacia.\n"); // El dominio son listas no vacias.
    }
    if(final){
        lista->elem[lista->cant]++;
    }
    lista->elem[0]++;
}

void eliminar_de_lista(Lista* lista, int final){
    if(lista->cant == 0){
        printf("Error, lista vacia.\n");
    }
    
    if(final){
        lista->elem--;
    }
    else{
        for(int i = 0; i < lista->cant - 1; i++){
            lista->elem[i] = lista->elem[i + 1]; 
        }
        lista->elem--;
    }

}