#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdlib.h>
#include "structs.h"

int devolverint(int dato){
    return dato;
}

int main(){
    TablaHash tabla = tablahash_crear(5, (FuncionHasheadora)devolverint);
    tablahash_insertar(tabla, 2,1);
    tablahash_insertar(tabla, 1,2);
    tablahash_insertar(tabla, 3,1);
    tablahash_insertar(tabla, 3,2);
    tablahash_insertar(tabla, 3,3); tablahash_insertar(tabla, 3,4); tablahash_insertar(tabla, 3,5);
    tablahash_insertar(tabla, 4,3);

    printf("Es la tabla del ejercicio simetrica?: ");
    printf("%d \n", es_simetrica(tabla));
    tablahash_destruir(tabla);
    
    TablaHash tabla2 = tablahash_crear(2, (FuncionHasheadora)devolverint);
    tablahash_insertar(tabla2, 0,0);
    tablahash_insertar(tabla2, 1,1);
    printf("%d \n", es_simetrica(tabla2));



    return 0;
}