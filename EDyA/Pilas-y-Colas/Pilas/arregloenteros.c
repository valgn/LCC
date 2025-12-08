#include <stdio.h>
#include <stdlib.h>
#include "arregloenteros.h"

ArregloEnteros *arreglo_enteros_crear(int capacidad)
{
    ArregloEnteros *ptr = malloc(sizeof(ArregloEnteros));

    ptr->direccion = malloc(sizeof(int) * capacidad);

    ptr->capacidad = capacidad;

    return ptr;
}                       

void arreglo_enteros_destruir(ArregloEnteros *arreglo)
{
    free(arreglo->direccion);
    free(arreglo);
}

int arreglo_enteros_leer(ArregloEnteros *arreglo, int pos)
{
    if (pos < 0 || pos >= arreglo->capacidad)
    {
        perror("ERROR");
    }
    return *(arreglo->direccion + pos*4);
}

void arreglo_enteros_escribir(ArregloEnteros *arreglo, int pos, int dato)
{
    if (pos < 0 || pos > arreglo->capacidad)
    {
        perror("ERROR");
    }
    *(arreglo->direccion + pos) = dato;
}

int arreglo_enteros_capacidad(ArregloEnteros *arreglo)
{
    return arreglo->capacidad;
}

void arreglo_enteros_imprimir(ArregloEnteros *arreglo)
{
    for (int i = 0; i < arreglo->capacidad; i++)
    {
        printf("%d ", *(arreglo->direccion + i));
    }
    printf("\n");
}

void arreglo_enteros_ajustar(ArregloEnteros *arreglo, int capacidad)
{
    arreglo->direccion = realloc(arreglo->direccion, sizeof(int) * capacidad);
    arreglo->capacidad = capacidad;
}

void arreglo_enteros_insertar(ArregloEnteros *arreglo, int pos, int dato)
{

    if (pos > arreglo->capacidad || pos < 0)
    {
        perror("Error");
    }

    int capacidad = arreglo_enteros_capacidad(arreglo);
    arreglo_enteros_ajustar(arreglo, capacidad + 1);

    for (int i = arreglo->capacidad - 1; i >= pos; i--)
    {
        *(arreglo->direccion + i) = *(arreglo->direccion + i - 1);
        arreglo_enteros_imprimir(arreglo);
    }

    *(arreglo->direccion + pos) = dato;
}

void arreglo_enteros_eliminar(ArregloEnteros *arreglo, int pos)
{
    if (pos > arreglo->capacidad || pos < 0)
    {
        perror("Error");
    }

    // int capacidad = arreglo_enteros_capacidad(arreglo);
    // arreglo_enteros_ajustar(arreglo,capacidad-1);

    for (int i = pos; i < arreglo->capacidad; i++)
    {
        *(arreglo->direccion + i) = *(arreglo->direccion + i + 1);
        arreglo_enteros_imprimir(arreglo);
    }

    int capacidad = arreglo_enteros_capacidad(arreglo);
    arreglo_enteros_ajustar(arreglo, capacidad - 1);
}
