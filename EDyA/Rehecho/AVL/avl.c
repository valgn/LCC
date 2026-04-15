#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void (*FuncionDestructora)(void*);
typedef void*(*FuncionCopiadora)(void*);
typedef int (*FuncionComparadora)(void*, void*);
typedef void (*FuncionVisitante)(void*);

typedef struct _AVLNodo{
    void* dato;
    int altura;
    struct _AVLNodo* right, *left;
}AVLNodo;

typedef struct _AVL{
    AVLNodo* raiz;
    FuncionComparadora comp;
    FuncionCopiadora copy;
    FuncionDestructora destroy;
}ARBOL;

typedef ARBOL* AVL;

AVL crear_arbol(FuncionComparadora comp, FuncionCopiadora copy, FuncionDestructora destroy){
    AVL arbol = malloc(sizeof(ARBOL));
    arbol->raiz = NULL;
    arbol->comp = comp; arbol->copy = copy; arbol->destroy = destroy;
    return arbol;
}

AVLNodo* crear_nodo(void* dato, FuncionCopiadora copy){
    AVLNodo* nuevo = malloc(sizeof(AVLNodo));
    nuevo->dato = copy(dato);
    nuevo->left = nuevo->right = NULL;
    nuevo->altura = 0;
    return nuevo;
}

static int altura_nodo(AVLNodo* raiz){
    if(raiz == NULL) return -1;
    return raiz->altura;
}

static int getBalance(AVLNodo* raiz){
    if(raiz == NULL) return 0;
    int balance = altura_nodo(raiz->right) - altura_nodo(raiz->left);
    return balance;
}

static int maxChildHeight(AVLNodo* raiz){
    if(raiz == NULL) return 0;
    int izq = altura_nodo(raiz->left);
    int der = altura_nodo(raiz->right);
    return izq > der ? izq : der;
}

AVLNodo* leftRotation(AVLNodo* raiz){
    AVLNodo* rightChild = raiz->right;
    assert(rightChild != NULL);
    raiz->right = rightChild->left;
    rightChild->left = raiz;
    
    raiz->altura = 1 + maxChildHeight(raiz);
    rightChild->altura = 1 + maxChildHeight(rightChild);
    
    return rightChild;
}

AVLNodo* rightRotation(AVLNodo* root){
    AVLNodo* leftChild = root->left;
    assert(leftChild);
    root->left = leftChild->right;
    leftChild->right = root;
    
    root->altura = 1 + maxChildHeight(root);
    leftChild->altura = 1 + maxChildHeight(leftChild);
    
    return leftChild;
}

AVLNodo* nodo_rebalancear(AVLNodo* raiz){
    if(getBalance(raiz) == 2){
        if(getBalance(raiz->right) == -1) raiz->right = rightRotation(raiz->right);
        raiz = leftRotation(raiz);
    }
    else if(getBalance(raiz) == -2){
        if(getBalance(raiz->left) == 1) raiz->left = leftRotation(raiz->left);
        raiz = rightRotation(raiz);
    }
    else return raiz;
    return raiz;
}

void destruir_nodo(AVLNodo* raiz, FuncionDestructora destroy){
    if(raiz == NULL){
        return;
    }
    destruir_nodo(raiz->left, destroy);
    destruir_nodo(raiz->right, destroy);
    destroy(raiz->dato);
    free(raiz);
    return;
}
void destruir_arbol(AVL arbol){
    destruir_nodo(arbol->raiz, arbol->destroy);
    free(arbol);
}

AVLNodo* insertar(AVLNodo* raiz, void* dato, FuncionCopiadora copy, FuncionComparadora comp, FuncionDestructora destroy){
    if(raiz == NULL){
        return crear_nodo(dato, copy);
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
    raiz->altura = 1 + maxChildHeight(raiz);
    return nodo_rebalancear(raiz);
}

void avl_insertar(AVL arbol, void* dato){
    arbol->raiz = insertar(arbol->raiz, dato, arbol->copy, arbol->comp, arbol->destroy);
}

AVLNodo* menor(AVLNodo* raiz){ // No me fijo si es NULL ya que la funcion se usa en el caso de que hayan dos hijos, raiz no va a
    //ser NULL
    AVLNodo* temporal = raiz;
    while(temporal->left != NULL){
        temporal = temporal->left;
    }
    return temporal;
}

AVLNodo* eliminar(AVLNodo* raiz, void* dato, FuncionComparadora comp, FuncionDestructora destroy){
    if(raiz == NULL) return NULL;
    int c = comp(dato, raiz->dato);
    if(c < 0) raiz->left = eliminar(raiz->left, dato, comp, destroy);
    else if (c > 0) raiz->right = eliminar(raiz->right, dato, comp, destroy);
    else{
        if(!raiz->left || !raiz->right){
            AVLNodo* sucesor = raiz->left  == NULL ? raiz->right : raiz->left;
            destroy(raiz->dato); free(raiz);
            return sucesor;
        }
        else{
            AVLNodo* sucesor = raiz->right;
            sucesor = menor(sucesor);
            
            void* copia_dato = sucesor->dato;
            sucesor->dato = raiz->dato;
            raiz->dato = copia_dato;
            raiz->right = eliminar(raiz->right, sucesor->dato, comp, destroy);

            raiz->altura = 1 + maxChildHeight(raiz);
            
            return nodo_rebalancear(raiz); 
        }
    }
    raiz->altura = 1 + maxChildHeight(raiz);
    return nodo_rebalancear(raiz);
}
void avl_eliminar(AVL arbol, void* dato){
    arbol->raiz = eliminar(arbol->raiz, dato, arbol->comp, arbol->destroy);
}

void* buscar(AVLNodo* raiz, void* dato, FuncionComparadora comp){
    if(raiz == NULL) return NULL;

    int c = comp(dato, raiz->dato);
    if(c < 0) return buscar(raiz->left, dato, comp);
    else if (c>0) return buscar(raiz->right, dato, comp);
    else{
        return raiz->dato;
    }
}

void* bst_buscar(AVL arbol, void* dato){
    return buscar(arbol->raiz, dato, arbol->comp);
}

void nodo_recorrer(AVLNodo* raiz, FuncionVisitante visit){
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
    AVL arbol = crear_arbol(comparar_entero, copiar_entero, destruir_entero);
    
    printf("--- INSERCIÓN SECUENCIAL (Test de Balanceo) ---\n");
    // Insertamos del 1 al 10. Un ABB sería una línea recta.
    for(int i = 1; i <= 10; i++){
        avl_insertar(arbol, &i);
        printf("Insertado %d | Altura Raiz: %d | Balance Raiz: %d\n", 
               i, altura_nodo(arbol->raiz), getBalance(arbol->raiz));
    }

    printf("\n--- ÁRBOL FINAL (Debe verse equilibrado) ---\n");
    nodo_recorrer(arbol->raiz, imprimir_entero);

    printf("\n--- ELIMINACIÓN (Test de Rebalanceo) ---\n");
    // Borramos nodos clave para forzar rotaciones
    int a_borrar[] = {5, 6, 4}; // Borramos la raíz y nodos internos
    for(int i = 0; i < 3; i++){
        printf("Eliminando %d...\n", a_borrar[i]);
        avl_eliminar(arbol, &a_borrar[i]);
        nodo_recorrer(arbol->raiz, imprimir_entero);
        printf("--------------------------------\n");
    }

    destruir_arbol(arbol);
    return 0;
}