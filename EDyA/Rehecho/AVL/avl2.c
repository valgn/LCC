#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void (*FuncionDestructora)(void*);
typedef void*(*FuncionCopiadora)(void*);
typedef int (*FuncionComparadora)(void*, void*);
typedef void (*FuncionVisitante)(void*);

typedef struct _AVLNODO{
    void* data;
    int height;
    struct _AVLNODO* right, *left;
}AVLNodo;

typedef struct{
    AVLNodo* root;
    FuncionComparadora cmp;
    FuncionCopiadora copy;
    FuncionDestructora destroy;
}AVLTree;

typedef AVLTree* AVL;

AVL avlCreate(FuncionCopiadora copy, FuncionComparadora cmp, FuncionDestructora destroy){
    AVL avl = malloc(sizeof(AVLTree));
    avl->cmp = cmp, avl->copy = copy, avl->destroy = destroy;
    avl->root = NULL;
    return avl;
}

AVLNodo* nodeCreate(void* data, FuncionCopiadora copy){
    AVLNodo* node = malloc(sizeof(AVLNodo));
    node->data = copy(data);
    node->height = 0;
    node->left = node->right = NULL;
    return node;
}

void nodeDestroy(AVLNodo* root, FuncionDestructora destroy){
    if(root == NULL) return;
    nodeDestroy(root->left, destroy);
    nodeDestroy(root->right, destroy);
    destroy(root->data);
    free(root);
}
void avlDestroy(AVL tree){
    nodeDestroy(tree->root, tree->destroy);
    free(tree);
}

int getHeight(AVLNodo* root){
    if(root == NULL) return -1;
     return root->height; }

int maxHeightChildern(AVLNodo* root){
    if(root == NULL){
        return 0;
    }
    int left = getHeight(root->left), right = getHeight(root->right);
    return left < right ? right : left;
}

int getBalance(AVLNodo* root){
    if(root == NULL) return 0;
    return getHeight(root->right) - getHeight(root->left);
}

AVLNodo* leftRotation(AVLNodo* root){
    AVLNodo* right = root->right;
    assert(right != NULL);
    root->right = right->left;
    right->left = root;
    root->height = 1 + maxHeightChildern(root);
    right->height = 1 + maxHeightChildern(right);
    return right;
}

AVLNodo* rightRotation(AVLNodo* root){
    AVLNodo* left = root->left;
    assert(left != NULL);
    root->left = left->right;
    left->right = root;
    root->height = 1 + maxHeightChildern(root);
    left->height = 1 + maxHeightChildern(left);
    return left;
}



AVLNodo* rebalance(AVLNodo* root){
    if(getBalance(root) == 2){
        if(getBalance(root->right) == -1) root->right = rightRotation(root->right);
        root = leftRotation(root);
    }
    else if(getBalance(root) == -2){
        if(getBalance(root->left) == 1) root->left = leftRotation(root->left);
        root = rightRotation(root);
    }
    return root;
}

AVLNodo* nodeInsert(AVLNodo* root, void* data, FuncionCopiadora copy, FuncionComparadora cmp, FuncionDestructora destroy){
    if(root == NULL){
        AVLNodo* newNode = nodeCreate(data, copy);
        return newNode;
    }
    int c = cmp(data, root->data);
    if(c < 0){
        root->left = nodeInsert(root->left, data, copy, cmp, destroy);
    }
    else if(c > 0){
        root->right =  nodeInsert(root->right, data, copy, cmp, destroy);
    }
    else{
        destroy(root->data);
        root->data = copy(data);
        return root;
    }
    root->height = 1 + maxHeightChildern(root);
    return rebalance(root);
}

void avlInsert(AVL tree, void* data){
    tree->root = nodeInsert(tree->root, data, tree->copy, tree->cmp, tree->destroy);
}

void* nodeSearch(AVLNodo* root, void* data, FuncionComparadora comp){
    if(root == NULL) return NULL;
    int c = comp(data, root->data);
    if(c < 0) return nodeSearch(root->left, data, comp);
    else if(c > 0) return nodeSearch(root->right, data, comp);
    else return root->data;
}

AVLNodo* menor(AVLNodo* raiz){
    AVLNodo* izq = raiz;
    while(izq->left != NULL) izq = izq->left;
    return izq;
}

AVLNodo* nodeRemove(AVLNodo* root, void* data, FuncionComparadora cmp, FuncionDestructora destroy){
    if(root == NULL) return NULL;
    int c = cmp(data, root->data);
    if(c < 0) root->left = nodeRemove(root->left, data, cmp, destroy);
    else if(c > 0) root->right = nodeRemove(root->right, data, cmp, destroy);   
    else{
        if(!root->right || !root->left){
            AVLNodo* sucesor = root->right == NULL ? root->left : root->right;
            destroy(root->data); free(root); 
            return sucesor;
        }
        else{
            AVLNodo* sucesor = root->right;
            sucesor = menor(sucesor);
            
            void* copia_dato = sucesor->data;
            sucesor->data = root->data;
            root->data = copia_dato;
            
            root->right = nodeRemove(root->right, sucesor->data, cmp, destroy);
            root->height = maxHeightChildern(root) + 1;
            return rebalance(root);
        }
    }
    root->height = maxHeightChildern(root) + 1;
    return rebalance(root);
}
void avlRemove(AVL tree, void* data){
    tree->root = nodeRemove(tree->root, data, tree->cmp, tree->destroy);
}

void visitNode(AVLNodo* root, FuncionVisitante visit){
    if(root == NULL) return;
    visitNode(root->left, visit);
    visit(root->data);
    visitNode(root->right, visit);
}

void visitTree(AVL tree, FuncionVisitante visit){
    visitNode(tree->root, visit);
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
    AVL tree = avlCreate(copiar_entero, comparar_entero, destruir_entero);
    for(int i = 0; i<10; i++){
        int n = 20 - i;
        avlInsert(tree, &n);
    }
    visitTree(tree, imprimir_entero);
    puts("");
    int eliminar[7] = {17,4,20,50, 11, 14, 15};
    for(int i = 0; i<7; i++){
        printf("La raiz en esta iteracion es: %d\n", *(int*)tree->root->data);
        avlRemove(tree, &eliminar[i]);
    }
    visitTree(tree, imprimir_entero);
    puts("");

    avlDestroy(tree);
    return 0;
}