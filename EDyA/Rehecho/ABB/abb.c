#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void (*FuncionDestructora)(void*);
typedef void*(*FuncionCopiadora)(void*);
typedef int (*FuncionComparadora)(void*, void*);
typedef void (*FuncionVisitante)(void*);

typedef struct _ABBNodo{
    void* dato;
    struct _ABBNodo* right, *left;
}Anodo;

typedef struct _ABB{
    Anodo* raiz;
    FuncionComparadora comp;
    FuncionCopiadora copy;
    FuncionDestructora destroy;
}ABB;

typedef ABB* BST;

BST crear_arbol(FuncionComparadora comp, FuncionCopiadora copy, FuncionDestructora destroy){
    BST arbol = malloc(sizeof(ABB));
    arbol->raiz = NULL;
    arbol->comp = comp; arbol->copy = copy; arbol->destroy = destroy;
    return arbol;
}

void destruir_nodo(Anodo* raiz, FuncionDestructora destroy){
    if(raiz == NULL){
        return;
    }
    destruir_nodo(raiz->left, destroy);
    destruir_nodo(raiz->right, destroy);
    destroy(raiz->dato);
    free(raiz);
    return;
}
void destruir_arbol(BST arbol){
    destruir_nodo(arbol->raiz, arbol->destroy);
    free(arbol);
}

Anodo* insertar(Anodo* raiz, void* dato, FuncionCopiadora copy, FuncionComparadora comp, FuncionDestructora destroy){
    if(raiz == NULL){
        Anodo* nuevo = malloc(sizeof(Anodo));
        nuevo->dato = copy(dato); nuevo->left = nuevo->right = NULL;
        return nuevo;
    }
    int c = comp(dato, raiz->dato);
    if(c < 0){
        raiz->left = insertar(raiz->left, dato, copy, comp, destroy);
    }
    else if(c > 0){
        raiz->right = insertar(raiz->right, dato, copy, comp, destroy);
    }
    else{
        destroy(raiz->dato);
        raiz->dato = copy(dato);
        return raiz;
    }
    return raiz;
}
void bst_insertar(BST arbol, void* dato){
    arbol->raiz = insertar(arbol->raiz, dato, arbol->copy, arbol->comp, arbol->destroy);
}

Anodo* menor(Anodo* raiz){ // No me fijo si es NULL ya que la funcion se usa en el caso de que hayan dos hijos, raiz no va a
    //ser NULL
    Anodo* temporal = raiz;
    while(temporal->left != NULL){
        temporal = temporal->left;
    }
    return temporal;
}

Anodo* eliminar(Anodo* raiz, void* dato, FuncionComparadora comp, FuncionDestructora destroy){
    if(raiz == NULL) return NULL;
    int c = comp(dato, raiz->dato);
    if(c < 0) raiz->left = eliminar(raiz->left, dato, comp, destroy);
    else if (c > 0) raiz->right = eliminar(raiz->right, dato, comp, destroy);
    else{
        if(!raiz->left || !raiz->right){
            Anodo* sucesor = raiz->left  == NULL ? raiz->right : raiz->left;
            destroy(raiz->dato); free(raiz);
            return sucesor;
        }
        else{
            Anodo* sucesor = raiz->right;
            sucesor = menor(sucesor);
            
            void* copia_dato = sucesor->dato;
            sucesor->dato = raiz->dato;
            raiz->dato = copia_dato;
            raiz->right = eliminar(raiz->right, sucesor->dato, comp, destroy);
            
            return raiz; 
        }
    }
    return raiz;
}
void bst_eliminar(BST arbol, void* dato){
    arbol->raiz = eliminar(arbol->raiz, dato, arbol->comp, arbol->destroy);
}

void* buscar(Anodo* raiz, void* dato, FuncionComparadora comp){
    if(raiz == NULL) return NULL;

    int c = comp(dato, raiz->dato);
    if(c < 0) return buscar(raiz->left, dato, comp);
    else if (c>0) return buscar(raiz->right, dato, comp);
    else{
        return raiz->dato;
    }
}

void* bst_buscar(BST arbol, void* dato){
    return buscar(arbol->raiz, dato, arbol->comp);
}

void nodo_recorrer(Anodo* raiz, FuncionVisitante visit){
    if(raiz == NULL){
        return;
    }
    nodo_recorrer(raiz->left, visit);
    visit(raiz->dato);
    nodo_recorrer(raiz->right, visit);
    return;
}

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

int main(){
    BST arbol = crear_arbol(comparar_entero, copiar_entero, destruir_entero);
    for(int i = 2; i < 15; i++){
        bst_insertar(arbol, &i);
    }
    int dos = 2; int cato = 14;
    int medio = 7;
    int search = *(int*)bst_buscar(arbol, &medio);
    
    if(search) printf("%d\n", search);
    else printf("No se encontró el dato\n");

    bst_eliminar(arbol, &dos); bst_eliminar(arbol, &cato); bst_eliminar(arbol, &medio);
    
    int medio2 = 7;
    void* search2 = bst_buscar(arbol, &medio2);
    if(search2) printf("%d\n", *(int*)search2);
    else printf("No se encontró el dato\n");

    nodo_recorrer(arbol->raiz, imprimir_entero);
    puts("");

    destruir_arbol(arbol);
    return 0;
}