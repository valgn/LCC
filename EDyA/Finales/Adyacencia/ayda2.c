#include <stdio.h>
#include <stdlib.h> 

typedef struct _SNodo {
  int dato;
  struct _SNodo *sig;
} SNodo;

typedef SNodo* SList;

typedef struct {
    int n;      // Tamaño de la matriz
    SList *fila; // fila[i] es lista de columnas j donde A[i][j] = 1
} MatrizEsparsa;

int slist_vacia(SList lista) {
  return lista == NULL;
}


SList slist_crear() {
  return NULL;
}

SList slist_eliminar(SList lista, int dato){
    if(slist_vacia(lista))
    return lista;

    if(lista->dato == dato){
        SNodo* temp = lista;
        lista = lista->sig;
        free(temp);
        return lista;
    }

    else{
        SNodo* temp = lista;
        while(temp->sig != NULL && temp->sig->dato != dato){
            temp = temp->sig;
        }
        if(temp->sig == NULL) return lista;
        else{
            SNodo* borrar = temp->sig;
            temp->sig = temp->sig->sig;
            free(borrar);
            return lista;

        }
    }
}

void slist_destruir(SList lista) {
  SNodo *nodoAEliminar;
  while (lista != NULL) {
    nodoAEliminar = lista;
    lista = lista->sig;
    free(nodoAEliminar);
  }
}

SList slist_agregar_inicio(SList lista, int dato) {
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = lista;
  return nuevoNodo;
}


void slist_recorrer(SList lista) {
  for (SNodo *nodo = lista; nodo != NULL; nodo = nodo->sig)
    printf("%d", (nodo->dato));
}

int slist_buscar(SList lista, int dato){
    SNodo* temp = lista;
    while(temp != NULL && temp->dato != dato){
        temp = temp->sig;
    }
    if(temp == NULL){
        return 0;
    }
    else return 1;
}

MatrizEsparsa* matriz_crear(int len){
    MatrizEsparsa* m = malloc(sizeof(MatrizEsparsa));
    m->fila = malloc(sizeof(SList)*len);
    m->n = len;
    for(int i = 0; i < len; i++){
        m->fila[i] = NULL;
    }
    return m;
}

void matriz_destruir(MatrizEsparsa* m){
    for (int i = 0; i < m->n; i++)
    {
        slist_destruir(m->fila[i]);
    }
    free(m->fila);
    free(m);
}

void insertar(MatrizEsparsa *m, int i, int j) {
    if (!slist_buscar(m->fila[i], j))
        m->fila[i] = slist_agregar_inicio(m->fila[i], j);
}

void eliminar(MatrizEsparsa *m, int i, int j) {
    m->fila[i] = slist_eliminar(m->fila[i], j);
}

int es_simetrica(MatrizEsparsa *m) {
    for (int i = 0; i < m->n; i++) {
        SNodo *aux = m->fila[i];
        while (aux != NULL) {
            int j = aux->dato;
            if (!slist_buscar(m->fila[j], i))
                return 0;
            aux = aux->sig;
        }
    }
    return 1;
}

void aux(MatrizEsparsa* m, int i, int* visitados){
    SList actual = m->fila[i];
    while (actual != NULL){
        int j = actual->dato;
        if(visitados[j] == 0){
            visitados[j] = 1;
            aux(m, j, visitados);
        }
        actual = actual->sig;
    }
}


int r_kleen(MatrizEsparsa* m , int i){
    int* visitados = calloc(m->n, sizeof(int));
    aux(m, i, visitados);
    int total = 0;
    for(int k = 0; k < m->n; k++){
        if(visitados[k] == 1 && k!=i) total++;
    }
    free(visitados);
    return total;
}

int main() {

    // Crear matriz de 5x5
    MatrizEsparsa *m = matriz_crear(5);

    // Relaciones del ejercicio
    insertar(m, 0, 1); // Ana -> Beto
    insertar(m, 1, 0); // Beto -> Ana

    // Cecilia es amiga de todos
    insertar(m, 2, 0);
    insertar(m, 2, 1);
    insertar(m, 2, 2);
    insertar(m, 2, 3);
    insertar(m, 2, 4);

    // Dalma amiga de Cecilia
    insertar(m, 3, 2);

    // Zaira no es amiga de nadie

    // Mostrar si es simétrica
    printf("¿La matriz es simétrica? %d\n", es_simetrica(m));

    int i = 3;
    int a = r_kleen(m, i);
    printf("El indice %d tiene alcance a %d nodos.\n", i, a);
    matriz_destruir(m);

    return 0;
}