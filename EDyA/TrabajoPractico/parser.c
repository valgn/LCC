#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "funciones.h"

#define MAX_FUNCIONES 100

typedef struct
{
    char *nombre;
    Funcion funcion;
} InputFunc;

InputFunc funciones_definidas[MAX_FUNCIONES]; // Aca guardamos las funciones que definimos
int cant_funciones = 0;

int es_repe(char *token)
{
    return token[0] == '<' && token[strlen(token) - 1] == '>';
}

void sacar_simbolito(char *token, char *nombre)
{
    strncpy(nombre, token + 1, strlen(token) - 2);
    nombre[strlen(token) - 2] = '\0'; // me deshago del >
}

void procesar_linea(char *linea)
{
    while (isspace(*linea))
        linea++; // no importa si hay espacion al principio

    if (strncmp(linea, "deff", 4) == 0)
    {
        linea += 4;
        while (isspace(*linea)) linea++;

        // Nombre de la funcion
        char nombre[32];
        int i = 0;
        while (*linea != '=' && *linea != '\0')
        {
            if (!isspace(*linea))
            {
                nombre[i++] = *linea;
            }
            linea++;
        }

        nombre[i] = '\0';

        if (*linea == '=')
            linea++;
        while (isspace(*linea))
            linea++;

        // Array de compo
        Funcion *pasos = malloc(sizeof(Funcion) * 100); // Espacio para 100 funciones
        int cant = 0;

        while (*linea && *linea != ';')
        {
            char token[32];
            int j = 0;

            while (*linea && !isspace(*linea) && *linea != ';')
            {
                token[j++] = *linea++;
            }
            token[j] = '\0';

            if (j > 0)
            {

                if (es_repe(token))
                { // Caso si encontramos una repeticion
                    char nombre_rep[32];
                    sacar_simbolito(token, nombre_rep);

                    FuncionPrimitiva f = obtener_primitiva(nombre_rep);
                    if (!f)
                    {
                        printf("Error: La funcion primitiva no existe.\n ");        // LOOPEAR PARA CASO <Si Si Si>
                        free(pasos);
                        return;
                    }

                    Funcion rep = crear_primitiva(f);
                    pasos[cant++] = crear_repeticion(&rep);
                }
                else
                {
                    FuncionPrimitiva f = obtener_primitiva(token);
                    if (!f)
                    {
                        printf("Error: La funcion primitiva '%s' no existe.\n", token);
                        free(pasos);
                        return;
                    }
                    pasos[cant++] = crear_primitiva(f);
                }
            }
            while (isspace(*linea))
                linea++;
        }

        if (*linea != ';')
        {
            printf("Error: la sentencia debe terminar en ';'. \n");
            free(pasos);
            return;
        }

        Funcion nueva = crear_compuesta(pasos, cant);

        funciones_definidas[cant_funciones].nombre = strdup(nombre);
        funciones_definidas[cant_funciones].funcion = nueva;
        cant_funciones++;

        printf("La funcion llamada '%s' se ha creado.\n", nombre);
    }
}
