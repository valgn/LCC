#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "listas.h"

void imprimir_entero(void* dato){
    printf("%d ", *(int*)dato);
}

int comparar_entero(void* dato1, void* dato2){
    return *(int*)dato1 - *(int*)dato2;
}

void* copiar_entero(void* dato){
    int* val = malloc(sizeof(int));
    assert(val != NULL);
    *val = *(int*)dato;
    return val;
}

void destruir_entero(void* dato){
    free(dato);
}

int main(){
    ListaSimple lse = lse_crear(comparar_entero, copiar_entero, destruir_entero, imprimir_entero);
    ListaDoble lde = lde_crear(comparar_entero, copiar_entero, destruir_entero, imprimir_entero);

    for(int i = 0; i < 50; i++){
        lse_agregarInicio(lse, &i);
        lde_agregarInicio(lde, &i);
    }

    for(int i = 10; i < 20; i++){
        lse_eliminar(lse, &i);
        lde_eliminar(lde, &i);
    }
    int primero = 49;
    lse_eliminar(lse, &primero);
    lde_eliminar(lde, &primero);

    int ultimo = 0;
    lse_eliminar(lse, &ultimo);
    lde_eliminar(lde, &ultimo);

    int cien = 100;
    lse_agregarFinal(lse, &cien);

    lde_agregarFinal(lde, &cien);
    SortMerge(lse);

    lse_recorrer(lse);
    puts("");
    lde_recorrer(lde);
    puts("");
    printf("%d\n", lse->cantidad);
    imprimir_entero(lse->fin->dato);
    puts("");


    printf("%d\n", lde->cantidad);
    imprimir_entero(lde->fin->dato);
    puts("");

    lse_destruir(lse);
    lde_destruir(lde);
    return 0;
}