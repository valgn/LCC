#include <stdio.h>
#include <stdlib.h>

typedef struct _Nodo{
    int dato;
    int suma;
    struct _Nodo* sig;
}Lista;

Lista* agregarLista(Lista* list, int dato){
    Lista* nodo = malloc(sizeof(Lista));
    if(list != NULL) nodo->suma = dato + list->suma;
    else nodo->suma = dato;

    nodo->dato = dato;
    nodo->sig = list;
    list = nodo;
    return list;
}

Lista* determinateChange(int c){
    int xx = 20, x = 10, v = 5, ii = 1;
    int modified = c;
    Lista* lista = NULL;
        if((modified) >= 20){
            for(int i = 0; i<modified/xx; i++){
                lista = agregarLista(lista, xx);
            }
            modified = modified % xx;
        }
        if((modified) >= 10){
            for(int i = 0; i<modified/x; i++){
                lista = agregarLista(lista, x);
            }
            modified = modified % x;
        }
        if((modified) >= 5){
            for(int i = 0; i<modified/v; i++){
                lista = agregarLista(lista, v);
            }
            modified = modified % v;
        }
        if((modified) >= 1){
            for(int i = 0; i<modified/ii; i++){
                lista = agregarLista(lista, ii);
            }
            modified = modified % ii;
        }                
        return lista;
    }



int main(){
    int c = 734;
    Lista* lista = determinateChange(c);
    printf("%d\n", lista->suma);
    for(struct _Nodo* temp = lista; temp != NULL; temp = temp->sig){
        printf("%d ", temp->dato);
    }
    puts("");
    struct _Nodo* temp = lista;
    while(temp != NULL){
        struct _Nodo* actual = temp;
        temp = temp->sig;
        free(actual);
    }
}