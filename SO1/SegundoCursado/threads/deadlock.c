#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t lockA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockB = PTHREAD_MUTEX_INITIALIZER;

void* thread1_routine(void* arg) {
    pthread_mutex_lock(&lockA);
    printf("Thread 1: Locked A\n");
    sleep(1); // Force a context switch to allow Thread 2 to lock B
    
    printf("Thread 1: Waiting for B...\n");
    pthread_mutex_lock(&lockB); // Deadlock occurs here
    
    pthread_mutex_unlock(&lockB);
    pthread_mutex_unlock(&lockA);
    return NULL;
}

void* thread2_routine(void* arg) {
    pthread_mutex_lock(&lockB);
    printf("Thread 2: Locked B\n");
    sleep(1);
    
    printf("Thread 2: Waiting for A...\n");
    pthread_mutex_lock(&lockA); // Deadlock occurs here
    
    pthread_mutex_unlock(&lockA);
    pthread_mutex_unlock(&lockB);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1_routine, NULL);
    pthread_create(&t2, NULL, thread2_routine, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
