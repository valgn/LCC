#include <stdlib.h>
#include <stdio.h>
#include "bst.h"

BSTSet crear_bst(){
    return NULL;
}

BSTSet insertar_bst(BSTSet set, int dato){
    if(set == NULL){
        bstNodo* nodo = malloc(sizeof(bstNodo));
        nodo->dato = dato;
        nodo->izq = NULL;
        nodo->der = NULL;
        return nodo;
    }
    else{
        if(dato < set->dato){
            set->izq = insertar_bst(set->izq, dato);
        }
        else if(dato > set->dato){
            set->der = insertar_bst(set->der, dato);
        }
        else{
            return set;
        }
    }
    return set;
}

void imprimir_set(BSTSet set){
    if(set == NULL){
        return;
    }
    imprimir_set(set->izq);
    printf("%d ", set->dato);
    imprimir_set(set->der);
}

int buscar_dato(BSTSet set, int dato){
    if(set == NULL){
        return 0;
    }
    if(set->dato == dato) return 1;
    if(dato < set->dato) return buscar_dato(set->izq, dato);
    else return buscar_dato(set->der, dato);
}

int longitud_set(BSTSet set){
    if(set == NULL){
        return 0;
    }
    return 1 + longitud_set(set->izq) + longitud_set(set->der);
}

void dfs(BSTSet set, BSTSet set2, BSTSet* res){
    if(set == NULL) return;
    
    if(buscar_dato(set2, set->dato)){
        *res = insertar_bst(*res, set->dato);
    }
    dfs(set->izq, set2, res);
    dfs(set->der, set2, res);
}

void destruir_bst(BSTSet set){
    if(set == NULL){
        return;
    }
    destruir_bst(set->izq);
    destruir_bst(set->der);
    free(set);
}



BSTSet interseccion(BSTSet set1, BSTSet set2){
    BSTSet res = crear_bst();

    dfs(set1, set2, &res);
    return res;
}

int lejanoBST(BSTSet set, int k){
    // El numero mas lejano a k esta en alguno de los dos extremos
    if(set == NULL) return 0;
    int izq;
    bstNodo* temp = set;
    while(temp->izq != NULL){
        temp = temp->izq;
    }
    izq = temp->dato;
    int der;
    bstNodo* temp2 = set;
    while(temp2->der != NULL){
        temp2 = temp2->der;
    }
    der = temp2->dato;

    if(abs(k - izq) >= abs(k - der)) return izq;
    else return der;
}

int main(){
    BSTSet set1 = crear_bst();
    BSTSet set2 = crear_bst();
    int nums[4] = {1,2,3,4};
    int nums2[4] = {3,4,5,6};
    for(int i = 0; i < 4; i++){
        set1 = insertar_bst(set1, nums[i]);
    }
    set1 = insertar_bst(set1, 2);
    for(int i = 0; i < 4; i++){
        set2 = insertar_bst(set2, nums2[i]);
    }
    printf("EL PRIMER SET ES { ");
    imprimir_set(set1);
    printf("}");
    puts("");
    printf("EL SEGUNDO SET ES { ");
    imprimir_set(set2);
    printf("}");
    puts("");
    BSTSet inter = interseccion(set1, set2);
    printf("La interseccion entre el set1 y el set2 es: { ");
    imprimir_set(inter);
    printf("}");
    puts("");
    
    printf("La raiz en el set1 es %d\n", set1->dato);
    int k=  lejanoBST(set1, 4);
    printf("El numero del arbol mas lejano a 4 es : %d\n", k);

    destruir_bst(set1);
    destruir_bst(set2);
    destruir_bst(inter);
    return 0;
}


