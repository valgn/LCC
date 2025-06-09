#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct _BTNodo {
  int dato;
  struct _BTNodo *left;
  struct _BTNodo *right;
}BTNodo;


typedef void (*FuncionVisitante)(int dato);
typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopia)(void *dato);
typedef int (*Predicado) (void *dato);
typedef void (*FuncionVisitanteExtra) (int dato, void *extra);

static void imprimir_entero(int dato){
    printf("%d ", dato);
}

typedef enum {
  BTREE_RECORRIDO_IN,
  BTREE_RECORRIDO_PRE,
  BTREE_RECORRIDO_POST
} BTreeOrdenDeRecorrido;

typedef struct _BTNodo *BTree;

typedef struct _AVL_Nodo
{
  int dato;
  struct _AVL_Nodo *left, *right;
  int altura;
} AVL_Nodo;

typedef struct _AVL_Nodo* AVL;

BTree btree_crear(){
    return NULL;
}

void btree_destruir(BTree tree){
    if(tree != NULL){
        btree_destruir(tree->left);
        btree_destruir(tree->right);
        free(tree);
    }
}

int btree_empty(BTree root){
    return root == NULL;
}

BTree btree_unir(int dato, BTree left, BTree right){
    BTree nuevoNodo = malloc(sizeof(struct _BTNodo));
    nuevoNodo->dato = dato;
    nuevoNodo->left = left;
    nuevoNodo->right = right;
    return nuevoNodo;
}

void btree_recorrer(BTree arbol, BTreeOrdenDeRecorrido orden, FuncionVisitante visit){
    if(arbol == NULL){
        return;
    }
    if(orden == BTREE_RECORRIDO_PRE) visit(arbol->dato);
    btree_recorrer(arbol->left, orden, visit);
    if(orden == BTREE_RECORRIDO_IN) visit(arbol->dato);
    btree_recorrer(arbol->right, orden, visit);
    if(orden == BTREE_RECORRIDO_POST) visit(arbol->dato);
}

int btree_nodos(BTree root){
    if(root == NULL){
        return 0;
    }
    return 1 + btree_nodos(root->left) + btree_nodos(root->right);
}

int btree_buscar(BTree root, int dato){
    if(root == NULL){
        return 0;
    }
    if(root->dato == dato){
        return 1;
    }
    return btree_buscar(root->left, dato) || btree_buscar(root->right, dato);
}

int btree_altura(BTree root){
    if(root == NULL){
        return -1;
    }
    int izq = 1 + btree_altura(root->left);
    int der = 1 + btree_altura(root->right);
    return der < izq ? izq : der;
}

// ABB con enteros

int bstree_buscar(BTree root, int dato){
    if(root == NULL){
        return 0;
    }
    if(dato < root->dato){
        bstree_buscar(root->left, dato);
    }
    else if(dato > root->dato){
        bstree_buscar(root->right, dato);
    }
    else return 1;
}

BTree crear_bstree(BTree root, int dato){
    if(root == NULL){
        BTree nuevoNodo = malloc(sizeof(struct _BTNodo));
        nuevoNodo->dato = dato;
        nuevoNodo->left = nuevoNodo->right = NULL;
        return nuevoNodo;
    }
    else if(dato < root->dato){
        root->left = crear_bstree(root->left, dato);
    }
    else if(dato > root->dato){
        root->right = crear_bstree(root->right, dato);
    }
    return root;
}

BTree eliminar_abb(BTree root, int dato) {
    if (root == NULL) {
        return NULL;
    }
    
    if (dato < root->dato) {
        root->left = eliminar_abb(root->left, dato);
    } 
    else if (dato > root->dato) {
        root->right = eliminar_abb(root->right, dato);
    } 
    else { // Nodo encontrado
        // Caso 1: Nodo hoja
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        // Caso 2: Un solo hijo
        else if (root->left == NULL || root->right == NULL) {
            BTree temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }
        // Caso 3: Dos hijos
        else {
            BTree sucesor = root->right;
            while (sucesor->left != NULL) {
                sucesor = sucesor->left;
            }
            root->dato = sucesor->dato;
            root->right = eliminar_abb(root->right, sucesor->dato);
        }
    }
    return root;
}

int raiz(BTree root){
    return root->dato;
}

int validar_abb(BTree root, int min, int max){
    if(root == NULL){
        return 1;
    }
    if(root->dato >= max || root->dato <= min){
        return 0;
    }
    return validar_abb(root->left, min, root->dato) && validar_abb(root->right, root->dato, max);
}

AVL avl_crear(){
    return NULL; 
}

static void avl_destruir(AVL root) {
    if(root != NULL) {
        avl_destruir(root->left);
        avl_destruir(root->right);
        free(root);
    }
}


int avl_buscar(AVL root, int dato) {
    if(root == NULL) {
        return 0;
    }
    else if(dato < root->dato) {
        return avl_buscar(root->left, dato);
    }
    else if(dato > root->dato) {
        return avl_buscar(root->right, dato);
    }
    else return 1;
}

int avl_altura(AVL root){
    return root == NULL ? -1 : root->altura;
}

int avl_altura_max(AVL root){
    if(root == NULL){
        return -1;
    }
    int izq = avl_altura_max(root->left);
    int der = avl_altura_max(root->right);
    return izq < der ? der : izq;
}

int factor_balance(AVL root){
    int factor = avl_altura_max(root->right) - avl_altura_max(root->left);
    return factor;
}

AVL rotacion_simple_izq(AVL root) {
    AVL hijoder = root->right;
    root->right = hijoder->left;
    hijoder->left = root;
    root->altura = 1 + avl_altura_max(root);
    hijoder->altura = 1 + avl_altura_max(hijoder);
    return hijoder;
}

AVL rotacion_simple_der(AVL root) {
    AVL hijoizq = root->left;
    root->left = hijoizq->right;
    hijoizq->right = root;
    root->altura = 1 + avl_altura_max(root);
    hijoizq->altura = 1 + avl_altura_max(hijoizq);
    return hijoizq;
}

AVL crear_nodo(int dato) {
    AVL nuevoNodo = malloc(sizeof(struct _AVL_Nodo));
    nuevoNodo->left = nuevoNodo->right = NULL;
    nuevoNodo->altura = 0;
    nuevoNodo->dato = dato;
    return nuevoNodo;
}

AVL rebalancear(AVL root) {
    if(factor_balance(root) == -2) {
        if(factor_balance(root->left) == 1) {
            root->left = rotacion_simple_izq(root->left);
        }
        root = rotacion_simple_der(root);  // Corregido: era rotacion_simple_izq
    }
    else if(factor_balance(root) == 2) {
        if(factor_balance(root->right) == -1) {
            root->right = rotacion_simple_der(root->right);
        }
        root = rotacion_simple_izq(root);  // Corregido: era rotacion_simple_der
    }
    return root;
}

AVL avl_insertar(AVL root, int dato){
    if(root == NULL){
        root = crear_nodo(dato);
    }
    if(dato < root->dato){
        root->left = avl_insertar(root->left, dato);
    }
    else if(dato > root->dato){
        root->right = avl_insertar(root->right, dato);
    }
    else return root;
    root->altura = 1 + avl_altura_max(root);
    
    return rebalancear(root);
}

AVL avl_menor(AVL root) {
    if(root == NULL) {
        return NULL;
    }
    AVL actual = root;
    while(actual->left != NULL) {
        actual = actual->left;
    }
    return actual;
}

AVL avl_eliminar(AVL root, int dato) {
    if(root == NULL) {
        return NULL;
    }
    if(dato < root->dato) {
        root->left = avl_eliminar(root->left, dato);
    }
    else if(dato > root->dato) {
        root->right = avl_eliminar(root->right, dato);
    }
    else {
        if(root->left == NULL || root->right == NULL) {
            AVL temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }
        else {
            AVL temp = avl_menor(root->right);
            root->dato = temp->dato;
            root->right = avl_eliminar(root->right, temp->dato);
        }
    }
    
    root->altura = 1 + avl_altura_max(root);
    return rebalancear(root);
}

int avl_abb(AVL root, int min, int max){
    if(root == NULL){
        return 1;
    }
    if(root->dato <= min || root->dato >= max){
        return 0;
    }
    return avl_abb(root->left, min, root->dato) && avl_abb(root->right, root->dato, max);
}

int avl_validar_alturabalance(AVL root){
    if(root == NULL){
        return 1;
    }
    int ret1 = avl_validar_alturabalance(root->left);
    int ret2 = avl_validar_alturabalance(root->right);
    if(ret1 && ret2){
        int altura = 1 + avl_altura_max(root);
        int factor = factor_balance(root);
        if((root->altura == altura) && (factor <= 1) && (factor >= -1)){
            return 1;
        }

    }
    return 0;
}

int avl_validar(AVL root){
    return avl_validar_alturabalance(root) && avl_abb(root, INT_MIN, INT_MAX);
}


static void avl_nodo_recorrer(AVL raiz,  BTreeOrdenDeRecorrido orden, FuncionVisitante visita)
{
  if (raiz != NULL)
  {
    if (orden == BTREE_RECORRIDO_PRE)
      visita(raiz->dato);
    avl_nodo_recorrer(raiz->left, orden, visita);
    if (orden == BTREE_RECORRIDO_IN)
      visita(raiz->dato);
    avl_nodo_recorrer(raiz->right, orden, visita);
    if (orden == BTREE_RECORRIDO_POST)
      visita(raiz->dato);
  }
}

AVL findkth(AVL root, int* k){
    if(root == NULL){
        return NULL;
    }
    AVL left = findkth(root->left, k);
    if(left != NULL){
        return left;
    }
    
    (*k)--;
    if(*k == 0){
        return root;
    }

    return findkth(root->right, k);

}

int contar_nodos(AVL root){
    if(root == NULL){
        return 0;
    }
    return 1 + contar_nodos(root->left) + contar_nodos(root->right);
}

double avl_mediana(AVL root){
    if(root == NULL){
        return 0;
    }
    int n = contar_nodos(root);
    if(n % 2 == 1){
        int k = (n+1) / 2;
        AVL medianaNodo = findkth(root, &k);
        return medianaNodo->dato;
    }
    else{
        //{1,2,3,4,5,6}
        int k1 = n / 2;
        int k2 = k1+1;

        int tmp1 = k1;
        AVL anterior = findkth(root, &tmp1);

        int tmp2 = k2;
        AVL siguiente = findkth(root, &tmp2);

        return (anterior->dato + siguiente->dato) / 2.0;

    }
}

int main(){
    BTree root = btree_crear();
    BTree left = btree_unir(5, NULL,NULL);
    BTree right = btree_unir(2, NULL,NULL);

    BTree abb = btree_crear();
    
    for(int i = 1; i<4; i++){
        abb = crear_bstree(abb, (i*2)+1);
    }
    for(int i = 1; i<4; i++){
        abb = crear_bstree(abb, i*2);
    }
    //printf("Soy la raiz: %d ", raiz(abb));
    puts("");
    
    abb = crear_bstree(abb, 9);
    
    // root = btree_unir(4, left, right);
    
    // abb = eliminar_abb(abb, 3);
    // abb = eliminar_abb(abb, 4);
    // abb = eliminar_abb(abb, 5);

    AVL avl = avl_crear();

    for(int i = 1; i<7; i++){
        avl = avl_insertar(avl, (i*2)+1);
    }
    for(int i = 1; i<7; i++){
        avl= avl_insertar(avl, i*2);
    }



   // printf("Soy la raiz: %d ", raiz(abb));

    //int esabb = validar_abb(root, INT_MIN, INT_MAX);
    
    //int altura = bstree_buscar(root, 6);
    puts("");
    //printf("%d ", esabb);
    int k = 2;
    AVL kesimo = findkth(avl, &k);
    printf("%d ",kesimo->dato);
    puts("");
    double mediana = avl_mediana(avl);
    printf("La mediana del arbol avl es: %lf", mediana);

    puts("");

    
    avl_nodo_recorrer(avl, BTREE_RECORRIDO_IN, imprimir_entero);

    //btree_recorrer(avl, BTREE_RECORRIDO_IN, (FuncionVisitante)imprimir_entero);
    puts("");


    return 0;
}