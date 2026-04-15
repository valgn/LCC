#include <stdio.h>
#include <stdlib.h>

typedef struct _Nodo {
    int dato;
    struct _Nodo *izq;
    struct _Nodo *der;
} Nodo;

typedef Nodo* Arbol;

// --- A IMPLEMENTAR POR VOS ---
int suma_nivel(Arbol a, int k);
void espejar(Arbol a);
int son_identicos(Arbol a, Arbol b);
// -----------------------------

// Función auxiliar para crear nodos rápido
Arbol nuevo(int dato, Arbol izq, Arbol der) {
    Arbol n = malloc(sizeof(Nodo));
    n->dato = dato; n->izq = izq; n->der = der;
    return n;
}

// Recorrido InOrden para verificar
void imprimir(Arbol a) {
    if(!a) return;
    imprimir(a->izq);
    printf("%d ", a->dato);
    imprimir(a->der);
}

int suma_nivel(Arbol a, int k){
    if(k == 0 && a) return a->dato;
    if(a == NULL) return 0;
    return suma_nivel(a->izq, k-1) + suma_nivel(a->der, k-1);
}

void espejar(Arbol a){
    if(a == NULL) return;
    espejar(a->izq);
    espejar(a->der);
    Nodo* temp = a->der;
    a->der = a->izq;
    a->izq = temp;
}

int son_identicos(Arbol a, Arbol b){
    if(a == NULL && b == NULL) return 1;
    if(a == NULL && b != NULL) return 0;
    if(a != NULL && b == NULL) return 0;
    if(a->dato != b->dato) return 0;
    return son_identicos(a->izq, b->izq) & son_identicos(a->der, b->der);
}

int main() {
    // Construimos un árbol manualmente:
    //       1
    //     /   \
    //    2     3
    //   / \     \
    //  4   5     6
    Arbol raiz = nuevo(1, 
                    nuevo(2, nuevo(4, NULL, NULL), nuevo(5, NULL, NULL)), 
                    nuevo(3, NULL, nuevo(6, NULL, NULL)));

    printf("--- TEST 1: Suma Nivel ---\n");
    printf("Nivel 0 (debe dar 1): %d\n", suma_nivel(raiz, 0));
    printf("Nivel 1 (debe dar 5): %d\n", suma_nivel(raiz, 1));
    printf("Nivel 2 (debe dar 15): %d\n", suma_nivel(raiz, 2));
    printf("Nivel 99 (debe dar 0): %d\n", suma_nivel(raiz, 99));

    printf("\n--- TEST 2: Identicos ---\n");
    Arbol copia = nuevo(1, 
                    nuevo(2, nuevo(4, NULL, NULL), nuevo(5, NULL, NULL)), 
                    nuevo(3, NULL, nuevo(6, NULL, NULL)));
    printf("Son identicos (1): %d\n", son_identicos(raiz, copia));
    
    // Modificamos la copia para que falle
    copia->izq->dato = 999;
    printf("Son identicos tras cambio (0): %d\n", son_identicos(raiz, copia));

    printf("\n--- TEST 3: Espejar ---\n");
    printf("Original (InOrden): "); imprimir(raiz); puts("");
    espejar(raiz);
    printf("Espejo   (InOrden): "); imprimir(raiz); puts("");
    // El espejo debería ser: 1 -> Der: 2 (Der: 4, Izq: 5), Izq: 3 (Izq: 6)
    // InOrden esperado: 6 3 1 5 2 4
    
    return 0;
}