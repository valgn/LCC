#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N 10
int visitante = 0;

int flag[2] = {0, 0};
// 0: no tengo intencion de entrar a la RC
// 1: tengo intencion de entrar a la RC
int turno=0;

// void *molinete1(void *ptr)
// {
//     for (int i = 0; i < N_VISITANTES; i++)
//     {
//         // lock
//         flag[0] = 1;
//         turno=2;
//         while (flag[1] == 1 && turno==2){;}
//         // region critica
//         visitante++;
//         // fin region critica - unlock

//         //printf("m1: visitantes: %d\n", visitante);
//         flag[0] = 0;
//     }

//     return ptr;
// }
// void *molinete2(void *ptr)
// {
//     for (int i = 0; i < N_VISITANTES; i++)
//     {
//         // lock
//         flag[1] = 1;
//         turno=1;
//         while (flag[0] == 1 && turno ==1){;}
//         // region critica
//         visitante++;
//         // fin region critica - unlock
        
//         //printf("m2: visitantes: %d\n", visitante);
//         //sleep(0.001);
//         flag[1] = 0;
//     }

//     return ptr;
// }

void * proc(void *arg) {
    int i;
    int id = arg - (void*)0;
    for (i = 0; i < N; i++) {
        int c;
        c = visitante;
        sleep(1);
        visitante = c + 1;
    }
    return NULL;
    }

int main()
{
    int v = 0;
    pthread_t m1;
    pthread_t m2;

    pthread_create(&m1, NULL, proc, &v);
    pthread_create(&m2, NULL, proc, &v);

    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("visitantes: %d\n", visitante);
    return 0;
}