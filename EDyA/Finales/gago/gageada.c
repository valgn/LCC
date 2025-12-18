#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct _vertex{
    unsigned i;
    struct _vertex* vecinos;
}Vertice;

typedef struct{
    Vertice* vertice;
}Casilla;

typedef struct _ListaAdy{
    Casilla* lista;
    unsigned cantVert;
}ListaAdy;
typedef ListaAdy* Grafo;

Grafo crear_grafo(int cantidad){
    Grafo grafo = malloc(sizeof(struct _ListaAdy));
    assert(grafo != NULL);
    grafo->cantVert = cantidad;
    grafo->lista = malloc(sizeof(Casilla) * cantidad);
    assert(grafo->lista != NULL);
    for(int i = 0; i<cantidad; i++){
        grafo->lista[i].vertice = NULL;
    }
    return grafo;
}

// Como es un grafo no dirigido hay que agregar dos conexiones
void agregar_conexion(Grafo grafo, int i, int j){
    assert(i < grafo->cantVert && j < grafo->cantVert);
    Vertice* vi = malloc(sizeof(Vertice));
    assert(vi != NULL);
    vi->i = j;
    vi->vecinos = grafo->lista[i].vertice;
    grafo->lista[i].vertice = vi;

    Vertice* vj = malloc(sizeof(Vertice));
    assert(vj != NULL);
    vj->i = i;
    vj->vecinos = grafo->lista[j].vertice;
    grafo->lista[j].vertice = vj;
}
// Borra la arista que une al vertice i con el j
void borrar_arista(Grafo grafo, int i, int j){
    assert(i < grafo->cantVert && j < grafo->cantVert);
   
    Vertice* vi = grafo->lista[i].vertice;
    if (vi == NULL) return;
  
    Vertice* temp = vi;
    if(vi->i == j){
        grafo->lista[i].vertice = vi->vecinos;
        free(temp);
    }else{
        while(temp->vecinos != NULL && temp->vecinos->i != j) temp = temp->vecinos;
        if(temp->vecinos == NULL) return;
        else{
            Vertice* borrar = temp->vecinos;
            temp->vecinos = temp->vecinos->vecinos;
            free(borrar);
        }        
    }

    Vertice* vj = grafo->lista[j].vertice;
    if (vj == NULL) return;

    temp = vj;
    if(vj->i == i){
        grafo->lista[j].vertice = vj->vecinos;
        free(temp);
    }else{
        while (temp->vecinos != NULL && temp->vecinos->i != i) temp = temp->vecinos;
        if(temp->vecinos == NULL) return;
        else{
            Vertice* borrar = temp->vecinos;
            temp->vecinos = temp->vecinos->vecinos;
            free(borrar);            
        }
    }
}

void borrar_vertice(Grafo grafo, int i){
    if(grafo->lista[i].vertice == NULL) return;
    borrar_arista(grafo, i, grafo->lista[i].vertice->i);
    borrar_vertice(grafo, i);
}

void recorrer(Grafo grafo, int i, int* visitados){
    // 1. PRIMERO: Marcar y procesar el nodo ACTUAL (i)
    // Esto arregla el problema del nodo 0 y simplifica el while
    visitados[i] = 1;
    printf("%d ", i);

    Vertice* actual = grafo->lista[i].vertice;
    
    // 2. DESPUES: Recorrer vecinos
    while(actual != NULL){
        // Si el vecino NO fue visitado, entramos (recursión)
        // Ya no hace falta marcarlo acá, se marcará al entrar a la función
        if(visitados[actual->i] == 0){
            recorrer(grafo, actual->i, visitados);
        }
        actual = actual->vecinos;
    }
}


void dfs(Grafo grafo){
    assert(grafo != NULL);
    int* visitados = calloc(grafo->cantVert,sizeof(int));
    for(int i = 0; i < grafo->cantVert; i++){
        if(visitados[i] == 0)
        recorrer(grafo, i, visitados);
    }
    free(visitados);
}

int main(){
    Grafo grafo = crear_grafo(4);
    agregar_conexion(grafo, 0,3);
    agregar_conexion(grafo, 0,1);
    agregar_conexion(grafo, 0,2);
    agregar_conexion(grafo, 2,3);

    dfs(grafo);



    return 0;
}


