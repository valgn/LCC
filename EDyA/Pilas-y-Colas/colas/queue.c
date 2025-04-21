#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

Cola cola_crear(){
    return glist_crear();
}

void cola_destruir(Cola cola, FuncionDestructora destroy){
    glist_destruir(cola, destroy);
}

int cola_es_vacia(Cola cola){
    return (cola == NULL);
}

void* cola_inicio(Cola cola){
    return cola->frente->data;
}
