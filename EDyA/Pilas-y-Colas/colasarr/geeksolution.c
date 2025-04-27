// C Program to implement queue using arrays
#include <stdio.h>
// Define the maximum size for the queue
#define MAX_SIZE 10

// Define a structure for the queue
struct Queue {
    int queue[MAX_SIZE];
    int front;
    int rear;
};

// Function to initialize the queue
void initializeQueue(struct Queue *q) {
    q->front = -1;
    q->rear = -1;
}

// Function to check if the queue is empty
int isEmpty(struct Queue *q) {
    return (q->front == -1);
}

// Function to check if the queue is full
int isFull(struct Queue *q) {
    return (q->rear == MAX_SIZE - 1);
}

// Function to insert an element into the queue
void enqueue(struct Queue *q, int data) {
    if (isFull(q)) {
        printf("Queue is full\n");
        return;
    }
    if (isEmpty(q)) {
        q->front = 0;
    }
    q->rear++;
    q->queue[q->rear] = data;
    printf("Enqueued %d in queue\n", data);
}

// Function to remove an element from the queue
int dequeue(struct Queue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return -1;
    }
    int data = q->queue[q->front];
    // If the queue is empty reset the pointers
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front++;
    }
    printf("Deleted element: %d\n", data);
    return data;
}

// Function to display the elements of the queue
void display(struct Queue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    for (int i = q->front; i <= q->rear; i++) {
        printf("%d ", q->queue[i]);
    }
    printf("\n");
}

int main() {
    // Initialize a queue
    struct Queue q;
    initializeQueue(&q);

    enqueue(&q, 1);
    enqueue(&q, 2);
    enqueue(&q, 3);
    enqueue(&q, 3);
    enqueue(&q, 3);
    enqueue(&q, 3);
    enqueue(&q, 3);
    enqueue(&q, 3);
    enqueue(&q, 3);
    enqueue(&q, 3);
    enqueue(&q, 3);
    enqueue(&q, 3);

    printf("Elements in the queue after enqueue operation: ");
    display(&q);

    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    dequeue(&q);
    
    printf("Elements in the queue after dequeue operation: ");
    display(&q);

    return 0;
}