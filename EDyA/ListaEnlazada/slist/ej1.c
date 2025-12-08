#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bubble_sort(float arreglo[], int longitud)
{
    for (int iter = 0; iter < longitud - 1; iter++)
    {
        for (int i = 0; i < longitud - iter - 1; i++)
        {
            if (arreglo[i] > arreglo[i + 1])
            {
                float aux = arreglo[i];
                arreglo[i] = arreglo[i + 1];
                arreglo[i + 1] = aux;
            }
        }
    }
}

float mediana(float *arreglo, int len)
{
    float *aux = malloc(sizeof(float) * len);
    for (int i = 0; i < len; i++)
    {
        aux[i] = arreglo[i];
    }
    bubble_sort(aux, len);
    float prom;
    if ((len % 2) == 0)
    {
        prom = (aux[len / 2] + aux[len / 2 - 1]) / 2;
    }
    else
    {
        prom = (aux[len / 2]);
    }
    free(aux);
    return prom;
}

int stringlen(char *str)
{
    int len;
    len = strlen(str);

    return len;
}

void stringreverse(char *str) 
{
    int izq = 0;
    int der = strlen(str) - 1;
    while (izq < der)
    {
        char temp = str[izq]; // hola temp = h, izq = a, der = h "aolh", temp = o, izq = l, der = temp = o "aloh"
        str[izq] = str[der];
        str[der] = temp;

        izq++;
        der--;
    }
}

int stringcompare(char *str1, char *str2)
{

    int bandera = 0;
    for (int i = 0; i < strlen(str1) || i < strlen(str2) && bandera == 0; i++)
    {
        if (str1[i] < str2[i])
        {
            bandera = -1;
        }
        else if (str1[i] > str2[i])
        {
            bandera = 1;
        }
    }
    return bandera;
}

int stringsubcadena(char *str1, char *str2)
{
    int index = 0;
    int i = 0;
    int j = 0;

    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if (len2 > len1)
        return -1;

    while (i < len1 && j < len2)
    {
        if (str1[i] == str2[j])
        {
            i++;
            j++;
            index = i;
        }
        else
        {
            i = i - j + 1;
            j = 0;
        }
    }
    if (j == len2)
    {
        return index - j;
    }
    else
    {
        return -1;
    }
}

void stringunir(char *arregloStrings[], int n, char *sep, char *res)
{
    int lenres = strlen(res);
    // res[lenres] = *sep;
    int pos = lenres;

    for (int i = 0; i < n; i++)
    {
        int lenarr = strlen(arregloStrings[i]);

        if (i <= n - 1)
        {
            int sepLen = strlen(sep);
            for (int k = 0; k < sepLen; k++)
            {
                res[pos] = sep[k];
                pos++;
            }
        }

        for (int j = 0; j < lenarr; j++)
        {

            res[pos] = arregloStrings[i][j];
            // res[lenres + 1 + strlen(arregloStrings[i])] = *sep;
            pos++;
        }
    }

    res[pos] = '\0';
}

typedef struct
{
    int *direccion;
    int capacidad;
} ArregloEnteros;


ArregloEnteros* arreglos_enteros_crear(int capacidad){
    ArregloEnteros* arreglo = (ArregloEnteros*)malloc(sizeof(ArregloEnteros));

    arreglo->direccion = (int*)malloc(capacidad * sizeof(int));

    arreglo->capacidad = capacidad;

    return arreglo;    
}

void arreglo_enteros_destruir(ArregloEnteros* arreglo){
    free(arreglo->direccion);
    free(arreglo);
}

int arreglo_enteros_leer(ArregloEnteros* arreglo, int pos){
    return arreglo->direccion[pos];
}

void arreglo_enteros_escribir(ArregloEnteros* arreglo, int pos, int dato){
    arreglo->direccion[pos] = dato;
}

int arreglo_enteros_capacidad(ArregloEnteros* arreglo){
    return arreglo->capacidad;
}

void arreglo_enteros_imprimir(ArregloEnteros* arreglo){
    for(int i = 0; i < arreglo->capacidad; i++){
        printf("%d", arreglo_enteros_leer(arreglo,i));
    }
}

void arreglo_enteros_ajustar(ArregloEnteros* arreglo, int capacidad){
    int* arreglo1 = (int*)realloc(arreglo->direccion, sizeof(int)*capacidad);

    arreglo->direccion = arreglo1;
    arreglo->capacidad = capacidad;
    
}

void arregloenterosinsertar(ArregloEnteros* arreglo, int pos, int dato){
    arreglo_enteros_ajustar(arreglo, arreglo->capacidad+1);
    arreglo->direccion[arreglo->capacidad-1] = dato;
    
    for(int i = arreglo->capacidad-1; i > pos;i--){
        int temp = arreglo->direccion[i-1];
        arreglo->direccion[i-1] = arreglo->direccion[i];
        arreglo->direccion[i]= temp;
    }
}

int main()
{
    // float arr1[] = {-1.0, 2.2, 2.9, 3.1, 3.5};
    // float arr2[] = {-1.0, 2.2, 2.9, 3.1};
    // char cadena[] = "Hola Mund";
    // printf("%d",stringcompare("azal","azul"));
    // printf("%d",strcmp("aaaaa","b"));
    printf("%d\n",stringsubcadena("12123","123"));
    // char res[50] = "hola";
    // char *arr[20] = {"tincho", "como", "estas"};
    // char sep[] = "/";
    // stringunir(arr, 3, sep, res);
    // printf("%s", res);

    // ArregloEnteros* arr = arreglos_enteros_crear(5);
    // arr->direccion[0] = 1;
    // arr->direccion[1] = 2;
    // arr->direccion[2] = 3;
    // arr->direccion[3] = 4;
    // arr->direccion[4] = 5;
    
    // arregloenterosinsertar(arr, 1,5);
    // arreglo_enteros_imprimir(arr);
    

    


    return 0;
}