#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "HybridTable.h"

/*
NO ME DI CUENTA DE USAR LA PLANTILLA DE GLIST, USE LA DE SLIST Y SE LA CAMBIE A VOID*, LOS NOMBRES LOS DEJE IGUAL
*/

void imprimir_entero(void* dato){
    printf("%d ", *(int*)dato);
}

int comparar_enteros(void* d1, void* d2){
    return *(int*)d1 - *(int*)d2;
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

unsigned hashear_entero(void* ent){
    return *(int*)ent;
}

int main(){
    HybridHash tabla = tablahash_crear(TABLE_CAP, copiar_entero, comparar_enteros, destruir_entero, hashear_entero);
    for(int i = 0; i < 1000; i++){
        tablahash_insertar(tabla, &i);
    }

    int cinco = 87;
    void* dato_buscar = tablahash_buscar(tabla, &cinco);
    if(dato_buscar == NULL) printf("No esta\n");
    else{
        imprimir_entero(dato_buscar);
    }
    
    int borrar = 1;
    tablahash_eliminar(tabla, &borrar);
    void* dato_buscar2 = tablahash_buscar(tabla, &borrar);
    
    if(dato_buscar2 == NULL) printf("No esta mas\n");
    else{
        imprimir_entero(dato_buscar2);
    }

    printf("PROBEMOS BUSCANDO ALGUNO QUE ESTE EN LA TABLA");
    puts("");
    int buscado = 5;
    void* busca = tablahash_buscar(tabla, &buscado);
    if(busca == NULL){
        printf("No se encontro\n");
    }
    else imprimir_entero(busca);

    printf("%d ", tabla->elems[0].cantidad);

    tabla_destruir(tabla);
    return 0;
}

