#ifndef ___EXTENSION_H___
#define ___EXTENSION_H___

typedef void (*FuncionVisitante)(void* dato);

typedef struct
{
    char *dni, *nombre, *descripcionCargo;
    int prioridad;
} _Invitado;

typedef _Invitado *Invitado;

typedef struct _Nodo
{
    Invitado invitado;
    struct _Nodo *sig, *ant;
} Nodo;

typedef Nodo *Lista;

Lista agregar(Invitado invitado, Lista listaDeInvitados);

Lista eliminar(Lista listaDeInvitados, int prioridadAprobada);

void recorrer(Lista listainv, FuncionVisitante func);

void imprimirInvitado(Invitado invitado);

#endif