#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "listas.h"

/* --- Funciones primitivas --- */
typedef Lista (*FuncionPrimitiva)(Lista lista);

/* --- Tipos de función posibles --- */
typedef enum {
    PRIMITIVA,
    COMPUESTA,
    REPETICION
} TipoFuncion;

/* --- Adelanto de tipo para casos recursivos --- */
typedef struct Funcion Funcion;

/* --- Función compuesta: array de sub-funciones --- */
typedef struct {
    Funcion* pasos; 
    int     cant;
} FuncionCompuesta;

/* --- Función repetición: puntero a sub-función --- */
typedef struct {
    Funcion* sub; 
} FuncionRepeticion;

/* --- Definición de la estructura Funcion --- */
struct Funcion {
    TipoFuncion tipo;
    union {
        FuncionPrimitiva   primi;  // caso PRIMITIVA
        FuncionCompuesta   comp;   // caso COMPUESTA
        FuncionRepeticion  repe;   // caso REPETICION
    };
};

/* --- Prototipos públicos --- */
FuncionPrimitiva obtener_primitiva(const char* nombre);

Funcion crear_primitiva(FuncionPrimitiva fptr);
Funcion crear_compuesta(Funcion* pasos, int cant);
Funcion crear_repeticion(Funcion* subf); 

Lista aplicar_funcion(Funcion f, Lista lista);
Lista aplicar_funcion_compuesta(const FuncionCompuesta comp, Lista lista);
Lista aplicar_funcion_repeticion(const FuncionRepeticion rep, Lista lista);

#endif /* FUNCIONES_H */