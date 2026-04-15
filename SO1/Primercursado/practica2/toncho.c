#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N 10000

int arr[N];
volatile int i=0;
int aCUM = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * proc(void *arg) {
    while(i<N){
        pthread_mutex_lock(&lock);
        aCUM+=arr[i++];
        pthread_mutex_unlock(&lock);
    }
    return NULL;
    }

int main()
{
    int v = 0;
    pthread_t m1;
    pthread_t m2;
    for(int j = 0; j<N;j++){arr[j]=j+1;}
    pthread_create(&m1, NULL, proc, &v);
    pthread_create(&m2, NULL, proc, &v);


    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("%d\n", aCUM);
    //printf("visitantes: %d\n", visitante);
    return 0;
}