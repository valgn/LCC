#ifndef STACK_UNBOUND_H
#define STACK_UNBOUND_H

#include <stdlib.h>
#include <pthread.h>

// A structure to represent a stack
// here we use a linked list to represent the unbound stack
struct StackNode {
    int data;
    struct StackNode* next;
};

struct ConcurrentStack{
    struct StackNode* stack;
    pthread_mutex_t mutex;
};

void concurrent_stack_init(struct ConcurrentStack* concurrentStack);

 
int concurrent_isEmpty(struct ConcurrentStack* root);
 
void concurrent_push(struct ConcurrentStack* root, int data);
 
int concurrent_pop(struct ConcurrentStack* root);
 
int concurrent_top(struct ConcurrentStack* root);



#endif /* CONCURRENT_STACK_H */