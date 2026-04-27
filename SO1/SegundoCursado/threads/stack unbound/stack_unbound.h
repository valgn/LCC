#ifndef STACK_UNBOUND_H
#define STACK_UNBOUND_H

#include <stdlib.h>
#include <pthread.h>

struct StackNode {
    int data;
    struct StackNode* next;
};

typedef struct _CStack{
    struct StackNode* prim;
    pthread_mutex_t lock;
} CStack;

struct StackNode* newNode(int data);
int isEmpty(CStack *pila);
void push(CStack *pila, int data);
int pop(CStack *pila);
int conc_pop(CStack *pila);
int top(CStack *pila);
void stackFree(CStack *pila);

#endif /* STACK_UNBOUND_H */