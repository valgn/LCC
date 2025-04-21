#include <stdio.h>
#include <stdlib.h>
#include "glist.h"
#include "gpilas.h"
#include "contacto.h"

// Función para imprimir un contacto
void imprimir_contacto(void *dato) {
    contacto_imprimir((Contacto*)dato);
}



int main() {
    // Crear la pila
    GPila pila = crear_gpila();

    Contacto *contactos[6];
    contactos[0] = contacto_crear("Pepe Argento", "3412695452", 61);
    contactos[1] = contacto_crear("Moni Argento", "3412684759", 60);
    contactos[2] = contacto_crear("Coqui Argento", "3415694286", 32);
    contactos[3] = contacto_crear("Paola Argento", "3416259862", 29);
    contactos[4] = contacto_crear("Maria Elena Fuseneco", "3416874594", 59);
    contactos[5] = contacto_crear("Dardo Fuseneco", "3416894526", 64);

    for (int i = 0; i < 6; ++i) {
        pila_apilar(pila, contactos[i], (FuncionCopia)contacto_copia);
    }

    printf("%d", gpila_es_vacia(pila));

    pila_desapilar(pila);
    pila_desapilar(pila);
    pila_desapilar(pila);
    pila_apilar(pila, contactos[2], (FuncionCopia)contacto_copia);
    pila_apilar(pila, contactos[2], (FuncionCopia)contacto_copia);
    pila_apilar(pila, contactos[2], (FuncionCopia)contacto_copia);


    GList lista = glist_crear();
    for(int i = 0; i < 6; i++){
        glist_agregar_inicio(&lista, contactos[i], (FuncionCopia)contacto_copia);
    }
    glist_recorrer(lista, (FuncionVisitante)contacto_imprimir);
    printf("\n");
    printf("\n");

    lista = dar_vuelta(lista, (FuncionCopia)contacto_copia);
    

    glist_recorrer(lista, (FuncionVisitante)contacto_imprimir);
    printf("\n");
    printf("\n");
    return 0;
}