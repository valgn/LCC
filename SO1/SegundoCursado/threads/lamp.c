#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#define NUM_VISITANTES 40000000
#define CANT_HILOS 10
int visitantes = 0;

int turno[CANT_HILOS] = {0};
int flag[CANT_HILOS] = {0};

#include <stdio.h>

int max(int* arr) {
    int n = sizeof(arr) / sizeof(arr[0]);
    int max = arr[0]; 
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}


void* molinete(void* thid){
    int id = *(int*)thid;
    for(int i = 0; i < NUM_VISITANTES; i++){
        flag[id] = 1;
        turno[id] = 1 + max(turno);
        flag[id] = 0;        
        for(int j = 0; j < CANT_HILOS; j++){
            while (flag[j]){;}
            
            while(turno[j] != 0 && (turno[j] < turno[id] || turno[id] == turno[j]) && (j < id))   {;}
            
            
        }

        visitantes++;

        flag[id] = 0;
    }
    
}

int main(){
    pthread_t hilos[CANT_HILOS];
    for(int i = 0; i < CANT_HILOS; i++) pthread_create(&hilos[i], NULL, molinete, (void*)&i);    
   



    for(int i = 0; i < CANT_HILOS; i++) pthread_join(hilos[i], NULL);  
    printf("Total visitates: %d\n", visitantes);
    return 0;
}