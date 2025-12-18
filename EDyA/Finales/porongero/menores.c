#include <stdio.h>
#include <stdlib.h>


typedef struct _Nodo {
    int valor;
    int cantidadMenores; // El dato que pide el ejercicio
    
    // --- LISTA 1: ORDEN DE SECUENCIA (Posición) ---
    struct _Nodo *sig;
    struct _Nodo *ant;
} Nodo;


typedef Nodo* ED;

typedef struct _SNodo{
    int valor;
    struct _SNodo* sig;
}Snodo;


ED crear_doble(){
    return NULL;
}

void insertar_pos(ED* lista, int valor, int k){
    Nodo* nuevo = malloc(sizeof(Nodo));
    nuevo->valor = valor;
    nuevo->ant = NULL;
    nuevo->sig = NULL;
    nuevo->cantidadMenores = 0;

    // CASO 1: INSERTAR AL PRINCIPIO (k=0)
    if(k == 0){
        nuevo->sig = *lista; // Apuntamos al viejo head
        if(*lista != NULL){
            (*lista)->ant = nuevo;
        }
        
        // CALCULO MATEMATICO (Solo hacia adelante)
        Nodo* actual = nuevo->sig;
        while(actual != NULL){
            // Si el nuevo (que esta antes) es menor que el actual, actual suma 1
            if(nuevo->valor < actual->valor) actual->cantidadMenores++;
            actual = actual->sig;
        }
        
        *lista = nuevo; // ACTUALIZAMOS EL PUNTERO ORIGINAL
        return;
    }

    // CASO 2: INSERTAR EN MEDIO O FINAL
    Nodo* temp = *lista;
    int i = 0;
    
    // Avanzamos hasta el anterior a k
    while(temp != NULL && i < k-1){
        temp = temp->sig;
        i++;
    }

    // PROTECCIÓN CONTRA SEGFAULT
    // Si temp es NULL, k es demasiado grande para esta lista.
    if(temp == NULL){ 
        free(nuevo); // No se pudo insertar, limpiamos
        return; 
    }

    // Si llegamos acá, temp es válido. Insertamos DESPUÉS de temp.
    nuevo->sig = temp->sig;
    if(temp->sig != NULL){
        temp->sig->ant = nuevo;
    }
    temp->sig = nuevo;
    nuevo->ant = temp;
   

    // --- CALCULO MATEMATICO ---
    
    // 1. Mirar hacia ATRAS (Mi cantidadMenores)
    Nodo* actual = nuevo->ant;
    int contador = 0;
    while(actual != NULL){
        if(actual->valor < valor) contador++;
        actual = actual->ant;
    }
    nuevo->cantidadMenores = contador;

    // 2. Mirar hacia ADELANTE (Actualizar a los demas)
    actual = nuevo->sig;
    while(actual != NULL){
        if(nuevo->valor < actual->valor){ // Tu logica estaba bien, esta es equivalente
            actual->cantidadMenores++;
        }
        actual = actual->sig;
    }
}

Snodo* buscar_cantidadmenores(ED lista, int cant){
    Nodo* temp = lista;
    Snodo* res = NULL;
    while(temp != NULL){
        if(temp->cantidadMenores == cant){
            Snodo* nuevo = malloc(sizeof(Snodo));
            nuevo->valor = temp->valor;
            nuevo->sig = res;
            res = nuevo;
        }
        temp = temp->sig;
    }
    return res;
}




int main(){
    ED jero = crear_doble();
    int valores[4] = {12, 30, 1, 2};
    for(int i = 0; i<4; i++){
        insertar_pos(&jero, valores[i], i);
    }
    Snodo* res = buscar_cantidadmenores(jero, 1);
    for(Snodo* temp = res; temp != NULL; temp = temp->sig){
        printf("%d ", temp->valor);
    }

    return 0;
}