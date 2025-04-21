#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

Cola cola_crear(){
    return NULL;
}

void cola_destruir(Cola cola, FuncionDestructora destroy){
    destroy(cola->lista->data);
    free(cola->lista);
    free(cola);
}

int cola_es_vacia(Cola cola){
    return (cola->lista == NULL);
}

void* cola_inicio(Cola cola){
    return cola->lista->frente->data;
}
