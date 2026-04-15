// C program for linked list implementation of stack
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "cstack.h"
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void concurrent_stack_init(struct ConcurrentStack* concurrentStack){
    concurrentStack->mutex = mutex;
    concurrentStack->stack = NULL;
}

int concurrent_isEmpty(struct ConcurrentStack* stack){
    return !stack->stack;
}

void concurrent_push(struct ConcurrentStack* concurrentStack, int data){
    pthread_mutex_lock(&concurrentStack->mutex);
    push(&concurrentStack->stack, data);
    pthread_mutex_unlock(&concurrentStack->mutex);
}

int concurrent_pop(struct ConcurrentStack* concurrentStack){
    pthread_mutex_lock(&concurrentStack->mutex);

    int dato = pop(&concurrentStack->stack);

    pthread_mutex_unlock(&concurrentStack->mutex);
}