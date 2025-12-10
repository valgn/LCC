#ifndef ___BST_H___
#define ___BST_H___

typedef struct bstNodo_
{
    int dato;
    struct bstNodo_ *der, *izq;
} bstNodo;

typedef bstNodo* BSTSet;


#endif