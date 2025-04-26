#include <stdio.h>
#include <stdlib.h>

typedef struct _SNodo{
    int dato;
    struct _SNodo* next;
}SNodo;

typedef SNodo *SOList;


SOList solistadd(int dato, SOList lista){
    SNodo* nuevoNodo = malloc(sizeof(struct _SNodo));
    nuevoNodo->dato = dato;
    if(!lista){
        nuevoNodo->next = NULL;
        return nuevoNodo;
    }
    if(lista->dato > nuevoNodo->dato){
        nuevoNodo->next = lista;
        return nuevoNodo;
    }
    SNodo* temp = lista;
    for(;temp->next != NULL && temp->next->dato <= dato; temp = temp->next);
    nuevoNodo->next = temp->next;
    temp->next = nuevoNodo;
    return lista;
}

void solistdestroy(SOList* lista){

    while(*lista != NULL){
        SNodo* temp = *lista;
        *lista = (*lista)->next;
        free(temp);
    }

}

int isEmpty(SOList lista){
    return lista == NULL;
}

void printearLista(SOList lista){
        for(SNodo* temp = lista; temp != NULL; temp = temp->next ){
        printf("%d ", temp->dato);
    }
}

void print_ordered(int* arr, int len){
    SOList lista = NULL;
    for(int i = 0;i < len;i++){
        lista = solistadd(arr[i], lista);
    }
    printearLista(lista);
    solistdestroy(&lista);
}


int main(){
    // SOList lista = NULL;
    // lista = solistadd(5, lista);
    // lista = solistadd(74, lista);
    // lista = solistadd(-2, lista);
    // lista = solistadd(4, lista);
    // lista = solistadd(7, lista);
    // lista = solistadd(5, lista);
    // lista = solistadd(1, lista);
    // lista = solistadd(-1, lista);
    // lista = solistadd(-43, lista);
    // lista = solistadd(6, lista);

    int len = 5;
    int arr[] = {3,4,5,1,2};
    print_ordered(arr, len);
    
    


    printf("\n");

    printf("\n");




    return 0;
}