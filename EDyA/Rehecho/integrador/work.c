#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CANT 1000

typedef struct{
    char* palabra;
    int estado; 
    int cantidad;
}HashEntry;

typedef struct{
    HashEntry* entry;
    int capacidad;
    int numElems;
}TablaHash;

typedef HashEntry* HeapNode;

typedef struct{
    HeapNode* nodo;
    int cantidad;
    int capacidad;
}MinHeap;

unsigned HashKr(char* s){
    unsigned hashval;
    for(hashval = 0; *s != '\0'; ++s){
        hashval = *s + 31 * hashval;
    }
    return hashval;
}

TablaHash* crear_tabla(int capacidad){
    TablaHash* tabla = malloc(sizeof(TablaHash));
    tabla->entry = calloc(capacidad, sizeof(HashEntry));
    tabla->numElems = 0; tabla->capacidad = capacidad;
    return tabla;
}

void insertar_contar(TablaHash* t, char* s){
    unsigned idx = HashKr(s) % t->capacidad;
    int i = idx;
    do{
        if(t->entry[idx].estado == 0){
            t->entry[idx].palabra = malloc(strlen(s) + 1);
            t->entry[idx].palabra = strcpy(t->entry[idx].palabra, s);
            t->entry[idx].cantidad = 1; 
            t->entry[idx].estado = 1;
            t->numElems++;
            return;
        }
        else if(strcmp(t->entry[idx].palabra, s) == 0){
            t->entry[idx].cantidad++;
            return;
        }

        idx = (idx+1) % t->capacidad;
    }while(i != idx);
}

void limpiar_palabra(char* dest, char* origen){
    int j = 0;
    for(int i = 0; origen[i] != '\0'; i++){
        if(isalpha(origen[i])){
            dest[j++] = tolower(origen[i]);
        }
    }
    dest[j] = '\0';
}

void cargar_archivo(TablaHash* t, char* archivo){
    FILE* fp = fopen(archivo, "r");
    
    if (fp == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", archivo);
        return;
    }
    char buffer_crudo[100];
    char buffer_limpio[100];

    while(fscanf(fp, "%s" ,buffer_crudo) == 1){
        limpiar_palabra(buffer_limpio, buffer_crudo);
        if(strlen(buffer_limpio) > 0) insertar_contar(t, buffer_limpio);
    }

    fclose(fp);
    printf("ARCHIVO CARGADO A LA TABLA HASH.\n");
    return;
}

void heapifyDown(MinHeap* h, int padre){
    int menor = padre;
    int hijoIzq = 2*padre + 1;
    int hijoDer = 2*padre + 2;
    if(hijoIzq < h->cantidad && h->nodo[hijoIzq]->cantidad < h->nodo[menor]->cantidad){
        menor = hijoIzq;
    }
    if(hijoDer < h->cantidad && h->nodo[hijoDer]->cantidad < h->nodo[menor]->cantidad){
        menor = hijoDer;
    }
    if(menor != padre){
        HeapNode temp = h->nodo[menor];
        h->nodo[menor] = h->nodo[padre];
        h->nodo[padre] = temp;
        heapifyDown(h, menor);
    }
}

void heapifyUp(MinHeap* h){
    int i = h->cantidad;
    while(i > 0){
        int padre = (i-1)/2;
        if(h->nodo[padre]->cantidad > h->nodo[i]->cantidad){
            HeapNode temp = h->nodo[i];
            h->nodo[i] = h->nodo[padre];
            h->nodo[padre] = temp;
            i = padre;
        }else break;
    }
}

void procesar_candidato(MinHeap* h, HashEntry* entrada){
    if(h->cantidad < h->capacidad){
        h->nodo[h->cantidad] = entrada;
        heapifyUp(h);
        h->cantidad++;
    }
    else{
        if(entrada->cantidad > h->nodo[0]->cantidad){
            h->nodo[0] = entrada;
            heapifyDown(h, 0);
        }
    }
}

void destruir_tabla(TablaHash* t){
    for(int i = 0; i<CANT; i++){
        if(t->entry[i].estado == 1){
            free(t->entry[i].palabra);
        }
    }
    free(t->entry);
    free(t);
}


int main(){
    TablaHash* t = crear_tabla(CANT);
    cargar_archivo(t, "texto.txt");


    // for(int i = 0; i < CANT;i++){
    //     if(t->entry[i].cantidad != 0){
    //         printf("El indice %d tiene la palabra %s, cantidad: %d\n", i, t->entry[i].palabra, t->entry[i].cantidad);
    //     }
    // }
    
    MinHeap h;
    h.cantidad = 0;
    h.capacidad = 10;
    h.nodo = malloc(sizeof(HeapNode) * 10);
   
    for(int i = 0; i<CANT;i++){
        if(t->entry[i].estado == 1){
            procesar_candidato(&h, &t->entry[i]);
        }
    }

    for(int i = 0; i<h.capacidad; i++){
        printf("%d. La palabra: %s, apariciones: %d\n", i+1, h.nodo[i]->palabra,h.nodo[i]->cantidad);
    }

    free(h.nodo);
    destruir_tabla(t);
    return 0;
}