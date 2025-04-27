#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _GNodo
{
    void *dato;
    struct _GNodo *sig;
} GNodo;

typedef struct
{
    GNodo *primero;
    GNodo *ultimo;
} GList;

typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopia)(void *dato);
typedef void (*FuncionVisitante)(void *dato);
typedef void *(*FuncionTransformar)(void *dato);

GList glist_crear()
{
    GList lista;
    lista.primero = NULL;
    lista.ultimo = NULL;
    return lista;
}
GList glist_agregar_final(GList lista, void *dato, FuncionCopia copia)
{
    GNodo *nuevo = malloc(sizeof(GNodo));
    nuevo->dato = copia(dato);
    nuevo->sig = NULL;
    if (lista.primero == NULL)
        lista.primero = nuevo;

    else
        lista.ultimo->sig = nuevo;

    lista.ultimo = nuevo;
    return lista;
}
void glist_destruir(GList lista, FuncionDestructora destroy)
{
    GNodo *temp;
    for (GNodo *nodo = lista.primero; nodo != NULL;)
    {
        temp = nodo;
        nodo = nodo->sig;
        destroy(temp->dato);
        free(temp);
    }
}

void glist_recorrer(GList lista, FuncionVisitante visit)
{
    for (GNodo *nodo = lista.primero; nodo != NULL; nodo = nodo->sig)
        visit(nodo->dato);
}
GList glist_map(GList lista, FuncionTransformar f, FuncionCopia c)
{
    GList nuevaLista = glist_crear();
    for (GNodo *nodo = lista.primero; nodo != NULL; nodo = nodo->sig)
    {
        // void* datoCopiado = c(nodo->dato);
        void *datoTransformado = f(nodo->dato);
        nuevaLista = glist_agregar_final(nuevaLista, datoTransformado, c);
    }
    return nuevaLista;
}

void *copy(void *dato)
{
    int *original = (int *)dato;
    int *copia = malloc(sizeof(int));
    *copia = *original;
    return copia;
}

void visit(void *dato)
{
    int *valor = (int *)dato;
    printf("%d ", *valor);
}

void visit2(void *dato)
{
    char *valor = (char *)dato;
    printf("%s ", valor);
}

void *transformar(void *dato)
{
    int *valor = (int *)dato;
    *valor = *valor * 2;
    return valor;
}

void *stringMayuscula(void *dato)
{
    char *string = (char *)dato;
    int len = strlen(string);

    char *copia = malloc(sizeof(char) * (len + 1));
    for (int i = 0; i < len; i++)
    {
        if(isupper(string[i])){
            copia[i] = string[i];
        }
        else if (islower(string[i]))
        {
            copia[i] = string[i] - 32;
        }
        else{
            copia[i] = string[i];
        }
            
        
    }
    copia[len] = '\0';
    return copia;
}

void *copia_string(void *dato)
{
    char *str = (char *)dato;
    char *nuevo = malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(nuevo, str);
    return nuevo;
}

void dest_string(void *dato) { 
    free(dato); 
}

int main()
{
    GList lista = glist_crear();
    char *str = "Esta ";
    lista = glist_agregar_final(lista, str, copia_string);
    str = "en ";
    lista = glist_agregar_final(lista, str, copia_string);
    str = "MaYuSculas ";
    lista = glist_agregar_final(lista, str, copia_string);
    str = "Aguante ";
    lista = glist_agregar_final(lista, str, copia_string);
    str = "merca./()";
    lista = glist_agregar_final(lista, str, copia_string);

    

    glist_recorrer(lista, (FuncionVisitante)visit2);

    lista = glist_map(lista, (FuncionTransformar)stringMayuscula, (FuncionCopia)copia_string);
    printf("\n");
    glist_recorrer(lista, (FuncionVisitante)visit2);

    glist_destruir(lista , dest_string);


    return 0;
}