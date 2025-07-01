#include <stdlib.h>
#include "listas.h"


Lista Oi(Lista lista){
    agregar_lista(&lista, 0, 0);
    return lista;
}

Lista Od(Lista lista){
    agregar_lista(&lista, 0, 1);
    return lista;
}

Lista Si(Lista lista){
    sucesor_lista(&lista, 0);
    return lista;
}

Lista Sd(Lista lista){
    sucesor_lista(&lista, 1);
    return lista;
}

Lista Dd(Lista lista){
    eliminar_de_lista(&lista, 1);
    return lista;
}

Lista Di(Lista lista){
    eliminar_de_lista(&lista, 0);
    return lista;
}




