#include "funciones.h"
#include <stdlib.h>
#include <string.h>

/* --- Primitivas --- */
Lista Oi(Lista lista) { agregar_lista(&lista, 0, 0); return lista; }
Lista Od(Lista lista) { agregar_lista(&lista, 0, 1); return lista; }
Lista Si(Lista lista) { sucesor_lista(&lista, 0); return lista; }
Lista Sd(Lista lista) { sucesor_lista(&lista, 1); return lista; }
Lista Di(Lista lista) { eliminar_de_lista(&lista, 0); return lista; }
Lista Dd(Lista lista) { eliminar_de_lista(&lista, 1); return lista; }

/* --- Mapeo nombre→primitiva --- */
FuncionPrimitiva obtener_primitiva(const char* nombre) {
    if (strcmp(nombre, "Oi") == 0) return Oi;
    if (strcmp(nombre, "Od") == 0) return Od;
    if (strcmp(nombre, "Si") == 0) return Si;
    if (strcmp(nombre, "Sd") == 0) return Sd;
    if (strcmp(nombre, "Di") == 0) return Di;
    if (strcmp(nombre, "Dd") == 0) return Dd;
    return NULL;
}

/* --- Creadores --- */
Funcion crear_primitiva(FuncionPrimitiva fptr) {
    Funcion f = { .tipo = PRIMITIVA };
    f.primi = fptr;
    return f;
}

Funcion crear_compuesta(Funcion* pasos, int cant) {
    Funcion f = { .tipo = COMPUESTA };
    f.comp.pasos = pasos;
    f.comp.cant  = cant;
    return f;
}

Funcion crear_repeticion(Funcion* subf) {
    /* hacemos una copia en heap para que el puntero sea estable */
    Funcion* copia = malloc(sizeof(Funcion));
    *copia = *subf;

    Funcion f = { .tipo = REPETICION };
    f.repe.sub = copia;
    return f;
}

/* --- Evaluadores --- */
Lista aplicar_funcion(Funcion f, Lista lista) {
    switch (f.tipo) {
        case PRIMITIVA:
            return f.primi(lista);
        case COMPUESTA:
            return aplicar_funcion_compuesta(f.comp, lista);
        case REPETICION:
            return aplicar_funcion_repeticion(f.repe, lista);
        default:
            return lista;
    }
}

Lista aplicar_funcion_compuesta(const FuncionCompuesta comp, Lista lista) {
    for (int i = 0; i < comp.cant; i++) {
        lista = aplicar_funcion(comp.pasos[i], lista);
    }
    return lista;
}

Lista aplicar_funcion_repeticion(const FuncionRepeticion rep, Lista lista) {
    while (lista.cant >= 2 &&
           lista.elem[0] != lista.elem[lista.cant - 1]) {
        lista = aplicar_funcion(*rep.sub, lista);
    }
    return lista;
}