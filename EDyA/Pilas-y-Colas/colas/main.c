#include <stdio.h>
#include <stdlib.h>
#include "contacto.h"
#include "queue.h"
#include "queueglist.h"

int main(){
    Cola cola = cola_crear();
    
    Contacto *contactos[6];
    contactos[0] = contacto_crear("Pepe Argento", "3412695452", 61);
    contactos[1] = contacto_crear("Moni Argento", "3412684759", 60);
    contactos[2] = contacto_crear("Coqui Argento", "3415694286", 32);
    contactos[3] = contacto_crear("Paola Argento", "3416259862", 29);
    contactos[4] = contacto_crear("Maria Elena Fuseneco", "3416874594", 59);
    contactos[5] = contacto_crear("Dardo Fuseneco", "3416894526", 64);

    //printf("%d", cola_es_vacia(cola));

    for (int i = 0; i < 6; i++) {
    encolar(cola, contactos[i], (FuncionCopia)contacto_copia);
    }

    Contacto dequeue = *(Contacto*)desencolar(cola);
    contacto_imprimir(&dequeue);

    Contacto dequeue2 = *(Contacto*)desencolar(cola);
    contacto_imprimir(&dequeue2);
    encolar(cola, &dequeue, (FuncionCopia)contacto_copia);
    printf("\n");
    cola_imprimir(cola, (FuncionVisitante)contacto_imprimir);


    return 0;
}