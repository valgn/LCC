#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extension.h"

int main(){
    Invitado invitado1 = malloc(sizeof(_Invitado));
    invitado1->dni = "46494432";
    invitado1->prioridad = 2;
    invitado1->descripcionCargo = "Presidente";
    invitado1->nombre = "Javier Milei";

    Invitado invitado2 = malloc(sizeof(_Invitado));
    invitado2->dni = "46432455";
    invitado2->prioridad = 1;
    invitado2->descripcionCargo = "Puto";
    invitado2->nombre = "TinchoKayser";

    Invitado invitado3 = malloc(sizeof(_Invitado));
    invitado3->dni = "5436634";
    invitado3->prioridad = 5;
    invitado3->descripcionCargo = "gay";
    invitado3->nombre = "Bruno";

    Invitado invitado4 = malloc(sizeof(_Invitado));
    invitado4->dni = "5436654";
    invitado4->prioridad = 7;
    invitado4->descripcionCargo = "Narco";
    invitado4->nombre = "Fulvio";
    
    Lista listainvitados = NULL;

    listainvitados = agregar(invitado1, listainvitados);
    listainvitados = agregar(invitado2, listainvitados);
    listainvitados = agregar(invitado3, listainvitados);
    listainvitados = agregar(invitado4, listainvitados);
    recorrer(listainvitados, (FuncionVisitante)imprimirInvitado);
    listainvitados = eliminar(listainvitados, 4);
    printf("\n");
    recorrer(listainvitados, (FuncionVisitante)imprimirInvitado);


    return 0;
}