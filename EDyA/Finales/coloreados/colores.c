#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>



typedef enum color{
    Rojo,
    Azul,
    Neutro
} Color;

typedef struct Nodo_{
    char* cdato;
    Color colorNodo;
    struct Nodo_ *der, *izq;
} Nodo;


typedef Nodo* ArbolBinarioColoreado;


ArbolBinarioColoreado crear_arbol(){
    return NULL;
}

ArbolBinarioColoreado insertar_arbol(ArbolBinarioColoreado der, ArbolBinarioColoreado izq, char* cdato, Color color){
    Nodo* nuevonodo = malloc(sizeof(Nodo));
    nuevonodo->der = der;
    nuevonodo->izq = izq;
    nuevonodo->cdato = cdato;
    nuevonodo->colorNodo = color;
    return nuevonodo;
}

void destruir_arbol(ArbolBinarioColoreado arbol){
    if(arbol == NULL){
        return;
    }
    destruir_arbol(arbol->izq);
    destruir_arbol(arbol->der);
    free(arbol);
}


void aux(ArbolBinarioColoreado raiz, int* rojo, int* azul){
    if(raiz == NULL){
        return;
    }
    aux(raiz->izq, rojo, azul);
    aux(raiz->der, rojo, azul);
    if(raiz->colorNodo == Rojo){
        (*rojo)++;
        return;
    }
    else if(raiz->colorNodo == Azul){
        (*azul)++;
        return;
    }
}

Color color(ArbolBinarioColoreado raiz){
    if(raiz == NULL) return Neutro;
    int cantroj = 0;
    int cantazul = 0;
    aux(raiz, &cantroj, &cantazul);
    if (cantroj > cantazul) return Rojo;
    if (cantazul > cantroj) return Azul;
    return Neutro;
}

void aux2(ArbolBinarioColoreado arbol, int* cantpuras){
    if(arbol->der == NULL && arbol->izq == NULL) {
        (*cantpuras)++;
        return;
    }
    if(arbol->der != NULL && arbol->colorNodo == arbol->der->colorNodo) aux2(arbol->der, cantpuras);
    if(arbol->izq != NULL && arbol->colorNodo == arbol->izq->colorNodo) aux2(arbol->izq, cantpuras);
}

int CantidadPuras(ArbolBinarioColoreado arbol){
    if(arbol == NULL){
        return 0;
    }
    int cantpuras = 0;
    aux2(arbol, &cantpuras);
    return cantpuras;
}

int main(){
    ArbolBinarioColoreado raiz = crear_arbol();
    ArbolBinarioColoreado der = insertar_arbol(NULL, NULL, "hola", Rojo);
    ArbolBinarioColoreado izq = insertar_arbol(NULL, NULL, "hola", Azul);
    ArbolBinarioColoreado padre = insertar_arbol(NULL, izq, "hola", Azul);
    raiz = insertar_arbol(der, padre, "hola", Azul);

    printf("El arbol tiene %d hojas puras\n", CantidadPuras(raiz));
    Color colore  = color(raiz);
    if(colore == Azul){
        printf("Es azul\n");
    }
    else if(colore == Rojo) printf("Es rojo\n");
    else printf("Es neutral.\n");
    destruir_arbol(raiz);


    return 0;
}