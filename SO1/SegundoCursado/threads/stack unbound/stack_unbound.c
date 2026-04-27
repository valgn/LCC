#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack_unbound.h"
 
struct StackNode* newNode(int data)
{
    struct StackNode* stackNode = (struct StackNode*) malloc(sizeof(struct StackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}
 
int isEmpty(CStack *pila)
{
    return !pila->prim;
}
 
void push(CStack *pila, int data)
{      
    // Optimización: El malloc toma tiempo y es local al hilo. Lo hacemos SIN candado.
    struct StackNode* stackNode = newNode(data);
    
    // SECCIÓN CRÍTICA
    pthread_mutex_lock(&pila->lock);
    stackNode->next = pila->prim;
    pila->prim = stackNode;
    pthread_mutex_unlock(&pila->lock);
}

int pop(CStack *pila)
{
    if (isEmpty(pila))
        return INT_MIN;
    struct StackNode* temp = pila->prim;
    pila->prim = pila->prim->next;
    int popped = temp->data;
    free(temp);
    return popped;
}

int conc_pop(CStack *pila){
    pthread_mutex_lock(&pila->lock);
    int ret = pop(pila);
    pthread_mutex_unlock(&pila->lock);
    return ret;
}
 
int top(CStack *pila)
{
    if (isEmpty(pila))
        return INT_MIN;
    return pila->prim->data;
}

void stackFree(CStack *pila)
{
  while(!isEmpty(pila))
  {
      pop(pila);
  }
}