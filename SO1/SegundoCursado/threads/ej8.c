#include <stdio.h>
#include <assert.h>
#include <pthread.h>

int arr[10000];

int len = sizeof(arr) / sizeof(int);
pthread_mutex_t lock;
int count = 0;

void *rutina(void *inicio)
{
    int final = len;
    if (*(int *)inicio == 0)
        final = len / 2;
    int in = *(int *)inicio;
    int suma_privada = 0;
    for (int i = in; i < final; i++)
    {
        suma_privada += arr[i];
    }
    pthread_mutex_lock(&lock);
    count += suma_privada;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main()
{
    for (int i = 0; i < 10000; i++)
        arr[i] = i + 1;
    int cero = 0;
    int mitad = len / 2;
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&t1, NULL, rutina, (void *)&cero);
    pthread_create(&t2, NULL, rutina, (void *)&mitad);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("%d \n", count);
    pthread_mutex_destroy(&lock);
    return 0;
}