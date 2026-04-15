#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void (*FuncionDestructora)(void*);
typedef int (*FuncionPredicado)(void*);
typedef void* (*FuncionCopiadora)(void*);
typedef void (*FuncionVisitante)(void*);
typedef int (*FuncionComparadora)(void*, void*);

/*
Funciones para main
*/
void destruir_entero(void* dato){
    free(dato);
}
void* copiar_entero(void* dato){
    int* val = malloc(sizeof(int));
    assert(val);
    *val = *(int*)dato;
    return val;
}

void imprimir_entero(void* dato){
    printf("%d ", *(int*)dato);
}
int comparar_entero(void* dato1, void* dato2){
    return *(int*)dato1 - *(int*)dato2;
}
int es_par(void* dato){
    return !(*(int*)dato % 2);
}
// ---------------------------

typedef struct _Nodo{
    void* dato;
    struct _Nodo* sig;
}Nodo;

typedef struct _Lista{
    Nodo* cabeza;
    Nodo* final;
    int quan;
}Lista;

Lista* crear_lista(){
    Lista* lista = malloc(sizeof(Lista));
    lista->cabeza = lista->final = NULL;
    lista->quan = 0;
    return lista;
}
void lista_destruir(Lista* lista, FuncionDestructora destroy){
    Nodo* actual = lista->cabeza;
    while(actual != NULL){
        Nodo* temp = actual;
        actual = actual->sig;
        destroy(temp->dato);
        free(temp);
    }
    free(lista);
}
void imprimir_lista(Lista* lista, FuncionVisitante visit){
    Nodo* temp = lista->cabeza;
    while(temp != NULL){
        visit(temp->dato);
        temp = temp->sig;
    }
    return;
}


Nodo* crear_nodo(void* dato, FuncionCopiadora copy){
    Nodo* nuevoNodo = malloc(sizeof(Nodo));
    nuevoNodo->dato = copy(dato);
    nuevoNodo->sig = NULL;
    return nuevoNodo;
}

void insertar_inicio(Lista* lista, void* dato, FuncionCopiadora copy){
    Nodo* nodo = crear_nodo(dato, copy);
    if(lista->cabeza == NULL){
        nodo->sig = lista->cabeza;
        lista->cabeza = nodo;
        lista->final = nodo;
    }
    else{
        nodo->sig = lista->cabeza;
        lista->cabeza = nodo;
    }
    lista->quan++;
    return;
}

void insertar_final(Lista* lista, void* dato, FuncionCopiadora copy){
    Nodo* nodo = crear_nodo(dato, copy);
    if(lista->final == NULL){
        nodo->sig = lista->cabeza;
        lista->cabeza = nodo;
        lista->final = nodo;
    }
    else{
        lista->final->sig = nodo;
        lista->final = nodo;
    }
    lista->quan++;
    return;
}

void lista_eliminar(Lista* lista, void* dato, FuncionDestructora destroy, FuncionComparadora comp){
    Nodo* actual = lista->cabeza;
    Nodo* ant = NULL;
    while(actual != NULL && comp(actual->dato, dato) != 0){
        ant = actual;
        actual = actual->sig;
    }
    if(actual == NULL) return;
    
    if(actual == lista->cabeza){
        lista->cabeza = actual->sig;
    }
    else{
        ant->sig = actual->sig;
    }

    if(actual == lista->final){
        lista->final = ant;
    }
    destroy(actual->dato);
    free(actual);
    lista->quan--;
    return;
}

Lista* lista_filtrar(Lista* lista, FuncionPredicado f, FuncionCopiadora copy){
    Lista* nueva_lista = crear_lista();
    Nodo* actual = lista->cabeza;
    while(actual != NULL){
        if(f(actual->dato)){
            insertar_inicio(nueva_lista, actual->dato, copy);
        }
        actual = actual->sig;
    }
    return nueva_lista;
}




int main(){
    Lista* list = crear_lista();
    int n = 2;
    for(int i = 0; i < 10; i++){
        insertar_inicio(list, &i, copiar_entero);
    }
    printf("------- La lista tiene %d elementos.\n",list->quan);
    int nueve = 9;
    int cer = 0;
    int medio = 5;
    lista_eliminar(list, &nueve, destruir_entero, comparar_entero);
    lista_eliminar(list, &cer, destruir_entero, comparar_entero);
    lista_eliminar(list, &medio, destruir_entero, comparar_entero);

    imprimir_lista(list, imprimir_entero);
    printf("------- La lista tiene %d elementos.\n",list->quan);
    puts("");

    
    lista_destruir(list, destruir_entero);

    return 0;
}