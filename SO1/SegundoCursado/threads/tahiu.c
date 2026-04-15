#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#define NUM_VISITANTES 4000000
int visitantes = 0;
int flag[2] = {0};
int turno = 0; // 0 m1 1 m2

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* molinete1(void* v){
    for(int i = 0; i < NUM_VISITANTES/2; i++){
        
        
        pthread_mutex_lock(&lock);
        visitantes++;
        //printf("contando: %d\n", visitantes);
        pthread_mutex_unlock(&lock);
        
    }
    

}

void* molinete2(void* v){
    for(int i = 0; i < NUM_VISITANTES/2; i++){
        
        
        pthread_mutex_lock(&lock);
        visitantes++;
        // printf("contando: %d\n", visitantes);
        pthread_mutex_unlock(&lock);
        
    }
    
}

int main(){
    pthread_t m1, m2;
    pthread_create(&m1, NULL, molinete1, NULL);
    pthread_create(&m2, NULL, molinete2, NULL);

    pthread_join(m1, NULL); pthread_join(m2, NULL);
    printf("Total visitates: %d\n", visitantes);
    return 0;
}