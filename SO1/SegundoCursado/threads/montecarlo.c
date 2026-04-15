#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// 1000000 x 1000 = 1 000 000 000 TARDA 3 SEGUNDOS
// 100000000 x 10 = 1 000 000 000 TARDA 3 SEGUNDOS
// 1000000000 x 1 = 1 000 000 000 TARDA 

#define CANT_PUNTOS 1000000000
#define RADIO 2
#define CANT_HILOS 1



int generar_checkear(int r, int n, unsigned int id){
    int count = 0;
    for(int i = 0; i < n; i++){
        float x = ((float)rand_r(&id) / RAND_MAX) * (2 * r);
        float y = ((float)rand_r(&id) / RAND_MAX) * (2 * r);
        float dx = (x-r)*(x-r);
        float dy = (y-r)*(y-r);
        if(dx + dy <= r*r) count++;
    }
    return count;
}

void* handler(void* a){
    int r = RADIO;
    int n = CANT_PUNTOS;
    unsigned int id = *(unsigned int*)a;
    int puntosInCircle = generar_checkear(r, n, id);
    *(int*)a = puntosInCircle;
    return NULL;
}

int main(){
    time_t t0 = time(NULL);
    printf("Hora actual: %s", ctime(&t0));
    
    srand(time(NULL));
    // for(int i = 0; i < 4; i++) printf("%.1f %.1f\n", puntos[i][0], puntos[i][1]);
    int* arr = malloc(sizeof(int) * CANT_HILOS);
    for(int i = 0; i < CANT_HILOS; i++) {
        arr[i] = i+1;
    }
    pthread_t threads[CANT_HILOS];
    for(int i = 0; i < CANT_HILOS; i++){
        pthread_create(&threads[i], NULL, handler, (void*)&arr[i]);
    }

    for(int i = 0; i < CANT_HILOS; i++){
        pthread_join(threads[i], NULL);
    }

    int inCircle = 0;
    float total = CANT_HILOS * CANT_PUNTOS;

    for(int i = 0; i < CANT_HILOS; i++){
        //printf("%d\n", *(int*)arr[i]);
        inCircle += arr[i];
    }
    printf("%d\n", inCircle);
    printf("%f\n", total);

    float pi = 4 * (inCircle / total);
    printf("%f\n",pi);
    time_t t1 = time(NULL);
    printf("Hora actual: %s", ctime(&t1));

    return 0;
}