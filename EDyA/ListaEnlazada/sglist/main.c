#include "sglist.h"
#include "contacto.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int comparar_contacto(Contacto* contacto, Contacto* contacto2){
    return(strcmp(contacto->nombre, contacto2->nombre));
}
int igual_contacto(Contacto* contacto, Contacto* contacto2){
    return (strcmp(contacto->nombre, contacto2->nombre)==0) && (strcmp(contacto->tel, contacto2->tel)==0) 
        && (contacto->edad == contacto2->edad);
}

int main(){

    GList lista = sglist_crear();

    Contacto *contactos[6];
    contactos[0] = contacto_crear("Pepe Argento", "3412695452", 61);
    contactos[1] = contacto_crear("Moni Argento", "3412684759", 60);
    contactos[2] = contacto_crear("Coqui Argento", "3415694286", 32);
    contactos[3] = contacto_crear("Paola Argento", "3416259862", 29);
    contactos[4] = contacto_crear("Maria Elena Fuseneco", "3416874594", 59);
    contactos[5] = contacto_crear("Dardo Fuseneco", "3416894526", 64);

     for (int i = 0; i < 6; ++i) {
        lista = sglist_insertar(lista, contactos[i], (FuncionCopia)contacto_copia, (FuncionComparadora)comparar_contacto);
        contacto_destruir(contactos[i]);
    }
    Contacto* c[1];
    c[0] = contacto_crear("tinchokaiser", "3416894526", 64);

    printf("%d\n", sglist_buscar(lista, c[0], (FuncionComparadora)igual_contacto));
    contacto_destruir(c[0]);
    printf("Lista:\n");
    sglist_recorrer(lista, (FuncionVisitante)contacto_imprimir);

    sglist_destruir(lista, (FuncionDestructora)contacto_destruir);

    return 0;
}