#ifndef __LISTAS_H__
#define __LISTAS_H__

typedef struct{
    int* elem;
    int cant; //cantidad
    int cap; //capacidad
}Lista;

Lista crear_lista(); //Crea una lista vacia


void destruir_lista(Lista* lista); //Destruye la lista;

void agregar_lista(Lista* lista, int elemento, int final); //Agrega un elemento al array al principio o al fina

void sucesor_lista(Lista* lista, int final); //Suma uno al elemento del inicio o al final de una lista

void eliminar_de_lista(Lista* lista, int final); // "Elimina" un elemento de la lista, el del inicio o del final

#endif