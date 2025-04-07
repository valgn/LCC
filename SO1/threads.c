#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
/*

*/

void* f(void* v){
    printf("thread id v: %d\n", *(int*)v);
    //sleep(2);
    pthread_exit(0);
    //return NULL;
}



int main(){
    pthread_t id;
    int v = 2;

    printf("Main creating thread...\n");


    pthread_create(&id, NULL, f,&v);

    //sleep(2);
    int *ptr;
    void *ptr2;
    //pthread_join(id,(void**) &ptr);
    pthread_join(id,(void**) &ptr2);
    //pthread_join(id,NULL);
    //printf("valor de salida %d\n",*(int*)(int**)&ptr);
    printf("valor de salida %d\n",*(int*)&ptr2);
    printf("Main corriendo...\n");

    return 0;
}
