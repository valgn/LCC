#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extension.h"

Lista agregar(Invitado invitado, Lista listaDeInvitados){
    Nodo* nuevoNodo = malloc(sizeof(Nodo));
    if (!nuevoNodo) return NULL;
    
    nuevoNodo->invitado = invitado;
    nuevoNodo->sig = NULL;
    nuevoNodo->ant = NULL;
    
    if (listaDeInvitados == NULL) {
        return nuevoNodo;
    }

    Nodo* temp = listaDeInvitados;
    if(invitado->prioridad < temp->invitado->prioridad){
        nuevoNodo->sig = temp;
        temp->ant = nuevoNodo;
        return nuevoNodo;
    }
    
    for(;temp->sig != NULL && temp->sig->invitado->prioridad <= invitado->prioridad; temp = temp->sig);
    nuevoNodo->sig = temp->sig;
    if(temp->sig != NULL) temp->sig->ant = nuevoNodo;
    temp->sig = nuevoNodo;
    nuevoNodo->ant = temp;
    return listaDeInvitados;
}

Lista eliminar(Lista listaDeInvitados, int prioridadAprobada){
    Nodo* temp = listaDeInvitados;

    while(temp != NULL){
        if(temp->invitado->prioridad >= prioridadAprobada){
            Nodo* eliminar = temp;
            if(eliminar->ant){
                eliminar->ant->sig = eliminar->sig;
            }
            if(eliminar->sig){
                eliminar->sig->ant = eliminar->ant;
            }
            if(eliminar == listaDeInvitados){
                listaDeInvitados = eliminar->sig;
            }
            
            temp=temp->sig;
            free(eliminar);
        }
        
        else temp = temp->sig;
    }
    return listaDeInvitados;
}

typedef void (*FuncionVisitante)(void* dato);


void recorrer(Lista listainv, FuncionVisitante func){
    for(;listainv != NULL; listainv = listainv->sig){
        func(listainv->invitado);
    }
}

void imprimirInvitado(Invitado invitado){
    printf("%s %s %s %d", invitado->nombre, invitado->dni, invitado->descripcionCargo, invitado->prioridad);
    printf("\n");
}




