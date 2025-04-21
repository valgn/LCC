#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Producer/consumer program illustrating conditional variables */

/* Size of shared buffer */
#define BUF_SIZE 10
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_hay_lugar = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_hay_elemento = PTHREAD_COND_INITIALIZER;

int buffer[BUF_SIZE];                                   /* shared buffer */
int add=0;                                                                              /* place to add next element */
int rem=0;                                                                              /* place to remove next element */
int num=0;                                                                              /* number elements in buffer */

/* Produce value(s) */
void *producer(void *param)
{
        int i;
        for (i=1; i<=2000; i++) {

            

            pthread_mutex_lock(&mutex);
            
            while(num == BUF_SIZE){
                printf("Productor en el wait\n");
                pthread_cond_wait(&cond_hay_lugar, &mutex);
            }

            /* Insert into buffer */
                if (num > BUF_SIZE) exit(1);    /* overflow */

                /* if executing here, buffer not full so add element */
                buffer[add] = i;
                add = (add+1) % BUF_SIZE;
                num++;
        printf ("producer: inserted %d\n", i);  fflush (stdout);
        pthread_cond_signal(&cond_hay_elemento);
        pthread_mutex_unlock(&mutex);
        }
        printf ("producer quiting\n");  fflush (stdout);
        return NULL;
}

/* Consume value(s); Note the consumer never terminates */
void *consumer(void *param)
{
        int i;
        while (1) {
            pthread_mutex_lock(&mutex);
            while(num == 0){
                printf("Consumidor en el wait\n");
                pthread_cond_wait(&cond_hay_elemento, &mutex);
            }
                if (num < 0) exit(1);   /* underflow */

                /* if executing here, buffer not empty so remove element */
                i = buffer[rem];
                rem = (rem+1) % BUF_SIZE;
                num--;
        printf ("Consume value %d\n", i);  fflush(stdout);
        pthread_cond_signal(&cond_hay_lugar);
        pthread_mutex_unlock(&mutex);

        }
        return NULL;
}

int main (int argc, char *argv[])
{
        pthread_t tid1, tid2;           /* thread identifiers */
        int i;

        /* create the threads; may be any number, in general */
        if (pthread_create(&tid1,NULL,producer,NULL) != 0) {
                fprintf (stderr, "Unable to create producer thread\n");
                exit (1);
        }
        if (pthread_create(&tid2,NULL,consumer,NULL) != 0) {
                fprintf (stderr, "Unable to create consumer thread\n");
                exit (1);
        }
        /* wait for created thread to exit */
        pthread_join(tid1,NULL);
        pthread_join(tid2,NULL);
        printf ("Parent quiting\n");
        return 0;
}

