#ifndef __FUNCIONES_H__
#define __FUNCIONES_H__


#include "listas.h"

/*       FUNCIONES PRIMITIVAS       */

typedef Lista (*FuncionPrimitiva)(Lista lista);

typedef Lista (*Funcion)(Lista);

Lista Oi(Lista lista); // Agrega un 0 a la izquierda de una lista

Lista Od(Lista lista); //Agrega un 0 a la derecha del todo de una lista

Lista Si(Lista lista); //Suma 1 al elemento de la izquierda de la lista

Lista Sd(Lista lista); //Suma 1 al elemento de la derecha del todo de la lista

Lista Di(Lista lista); //Elimina el elemento de la izquierda de la lista

Lista Dd(Lista lista); //Elimina el elemento de la derecha del todo de la lista


/*      Funcion Compuesta      */

typedef struct{
    FuncionPrimitiva* f;
    int cant;
}FuncionCompuesta; //Array de funciones primitivas

typedef struct{
    Funcion f;
}FuncionRepeticion;

typedef enum {
    PRIMITIVA,
    COMPUESTA,
    REPETICION
}TipoFuncion;

typedef struct {
    TipoFuncion tipo;
    union{
        FuncionPrimitiva primi;
        FuncionCompuesta comp;
        FuncionRepeticion repe;
    };

}Funcion;

#endif