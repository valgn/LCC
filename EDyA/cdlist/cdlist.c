#include <stdio.h>
#include <stdlib.h>
#include "cdlist.h"

CDList* crearlista(){
    CDList* lista = malloc(sizeof(CDList));
    lista->primero = NULL; return lista;
}

void agregar(CDList* lista, int dato){
    CDNodo* nuevonodo = malloc(sizeof(CDNodo));
    nuevonodo->dato = dato;

    if(lista->primero == NULL){
        nuevonodo->sig = nuevonodo;
        nuevonodo->ant = nuevonodo;
        lista->primero = nuevonodo;
    }
    else{
        CDNodo* ultimo;
        ultimo = lista->primero->ant;
        nuevonodo->sig = lista->primero;
        lista->primero->ant = nuevonodo;
        nuevonodo->ant = ultimo;
        ultimo->sig = nuevonodo;
        lista->primero = nuevonodo;

        // ultimo = lista->primero->ant;
        // nuevonodo->sig = lista->primero;
        // lista->primero->ant = nuevonodo;
        // ultimo->sig = nuevonodo;
        // nuevonodo->ant = ultimo;
        // lista->primero = nuevonodo;

    }

}

void recorrer(CDList* lista, FuncionVisitante v){
    CDNodo* temp = lista->primero;
    do{
        printf("%d, ", temp->dato); 
        temp = temp->sig;
    }while(temp != lista->primero);
}

void imprimir_entero(int n) {
    printf("%i", n);
}

void imprimir_doble(int n) {
    printf("%i", 2*n);
}


void recorrer2(CDList* lista, FuncionVisitante f) {
    CDNodo* temp = lista->primero;
    do{
        f(temp->dato);
        temp = temp->sig;
    }while(temp != lista->primero);
}


