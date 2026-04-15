#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#define VISITANTES_ESPERADOS 10000000
int visitantes = 0;

// 'volatile' obliga al compilador a leer de la RAM cada vez,
// evitando que optimice el while y cuelgue el hilo.
volatile int flags[2] = {0,0};
volatile int turn;

void* sumar(void* id){
    // 1. Ya no restamos -1. Los hilos son 0 y 1.
    int hilo = *(int*)id; 
    int el_otro = !hilo;  // Calculamos el ID del contrario (1 si somos 0, 0 si somos 1)

    for(int i = 0; i < VISITANTES_ESPERADOS; i++){
        // --- INICIO PROTOCOLO DE PETERSON ---
        flags[hilo] = 1;
        turn = el_otro;
        __sync_synchronize();  
        
        // 2. Barrera de Memoria. Obliga a la CPU a vaciar sus búferes de escritura 
        // antes de seguir. Impide el reordenamiento de memoria.

        while(flags[el_otro] && turn == el_otro) {
            // Esperar activamente (busy waiting)
        }
        
        // --- SECCIÓN CRÍTICA ---
        visitantes++;
        
        // --- FIN PROTOCOLO DE PETERSON ---
        flags[hilo] = 0;
    }
    return NULL;
}

int main(){
    pthread_t t1[2];
    
    // 3. Arreglo fijo para pasar los IDs de forma segura
    int ids[2] = {0, 1}; 
    
    for(int i = 0; i < 2; i++){
        // Pasamos la dirección de ids[i], que nunca cambiará de valor
        pthread_create(&t1[i], NULL, sumar, (void*)&ids[i]); 
    }
    
    for(int i = 0; i < 2; i++){
        pthread_join(t1[i], NULL);
    }
    
    printf("%d \n", visitantes); // ¡Ahora sí dará 2,000,000!
    return 0;
}