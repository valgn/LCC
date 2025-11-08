#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("La cantidad de argumentos es insuficiente\n");
    }
    else
    {
        int suma = 0;
        for (int i = 1; i < argc; i++)
        {
            suma = suma + atoi(argv[i]);
        }
        printf("Cantidad de argumentos: %d, suma de los argumentos: %d\n", argc, suma);
    }
    return 0;
}