#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "stack_unbound.h"

#define ELEMENT_COUNT 1000

CStack stack_conc; 

// Los hilos en C siempre deben tener esta firma: void* func(void* arg)
void *function1(void *arg) {
  for(int i = 0; i < ELEMENT_COUNT; ++i)
  {
    push(&stack_conc, i); // Pasamos la DIRECCIÓN de memoria
  }
  return NULL;
}

void *function2(void *arg) {
  for(int i = 0; i < ELEMENT_COUNT; ++i)
  {
    push(&stack_conc, i);
  }
  return NULL;
}

int main()
{
  // 1. ¡INICIALIZACIÓN PRIMERO! (Antes de crear hilos)
  stack_conc.prim = NULL;
  pthread_mutex_init(&stack_conc.lock, NULL);

  // 2. CREACIÓN DE HILOS
  pthread_t one, two; 
  pthread_create(&one, NULL, function1, NULL); 
  pthread_create(&two, NULL, function2, NULL);

  // 3. ESPERAR A LOS HILOS
  pthread_join(one, NULL);
  pthread_join(two, NULL);

  int counter = 0;

  // 4. LECTURA FINAL (Pasando por referencia)
  while(!isEmpty(&stack_conc))
  {
    counter++;
    conc_pop(&stack_conc);
  }

  printf("counter esperado: %d\n", ELEMENT_COUNT * 2);
  printf("counter final: %d\n", counter);

  // 5. LIMPIEZA
  stackFree(&stack_conc);
  pthread_mutex_destroy(&stack_conc.lock); // Siempre destruir el mutex al final
 
  return 0;
}