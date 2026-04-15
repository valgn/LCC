#include <stdio.h>
#include <stdlib.h>

typedef struct _Nodo {
    int destino;        // El ID del servidor conectado
    struct _Nodo *sig;
} Nodo;

typedef struct {
    Nodo **adyacencias; // Arreglo de punteros (Listas)
    int numVertices;    // Cantidad de servidores
    int numAristas;
} StructGrafo;

typedef StructGrafo* Grafo;

Grafo grafo_crear(int numVertices){
    Grafo grafo = malloc(sizeof(StructGrafo));
    grafo->adyacencias = malloc(sizeof(Nodo*) * numVertices);
    grafo->numAristas = 0; grafo->numVertices = numVertices;
    for(int i = 0; i<grafo->numVertices; i++){
        grafo->adyacencias[i] = NULL;
    }
    return grafo;
}

int existe_arista(Nodo* lista, int arista){
    int found = 0;
    Nodo* nodo = lista;
    for(; nodo != NULL && (nodo->destino-arista)!= 0; nodo = nodo->sig);
    return nodo == NULL ? 0 : 1;
}

void grafo_agregar_arista(Grafo grafo, int i, int j){
    // 1. Validación de índices (Fundamental para que no explote si i es > N)
    if (i < 0 || i >= grafo->numVertices) return;

    // 2. Verificar si ya existe (para evitar multigrafo)
    if(existe_arista(grafo->adyacencias[i], j)) return;

    // 3. Insertar al inicio (La lógica es la misma si estaba NULL o no)
    Nodo* nuevo = malloc(sizeof(Nodo));
    nuevo->destino = j;
    nuevo->sig = grafo->adyacencias[i];
    grafo->adyacencias[i] = nuevo;

    // 4. Actualizar contador
    grafo->numAristas++; 
}

void dfs(Grafo grafo, int i, int j, int* visitados, int* res){
    if(existe_arista(grafo->adyacencias[i], j)){
        (*res) = 1;
        return;
    } 
    visitados[i] = 1;
    for(Nodo* nodo = grafo->adyacencias[i]; nodo != NULL; nodo = nodo->sig){
        if(visitados[nodo->destino] == 0){
            dfs(grafo, nodo->destino, j, visitados, res);
        }
    }
    return;
}




int grafo_hay_camino(Grafo grafo, int i, int j){
    int* visitados = calloc(sizeof(int), grafo->numVertices); int res = 0;
    dfs(grafo, i, j, visitados, &res);
    free(visitados);
    return res;
}


// Función auxiliar para imprimir bonito (SI/NO)
void testear_camino(Grafo g, int origen, int destino, int esperado) {
    int resultado = grafo_hay_camino(g, origen, destino);
    printf("Camino %d -> %d: %s (Esperado: %s) -> %s\n", 
           origen, destino, 
           resultado ? "SI" : "NO", 
           esperado ? "SI" : "NO",
           (resultado == esperado) ? "PASSED ✅" : "FAILED ❌");
}

int main() {
    printf("=== TEST DE GRAFOS: BUSQUEDA DE CAMINOS ===\n");

    // 1. Crear Grafo de 6 nodos (0 al 5)
    Grafo g = grafo_crear(6);

    // 2. Armar la topología
    // Grupo Principal: 0 -> 1 -> 2 -> 0 (Ciclo) y 1 -> 3 (Rama)
    grafo_agregar_arista(g, 0, 1);
    grafo_agregar_arista(g, 1, 2);
    grafo_agregar_arista(g, 2, 0); // ¡Cuidado con el bucle infinito aquí!
    grafo_agregar_arista(g, 1, 3);

    // Grupo Aislado (Isla): 4 -> 5
    grafo_agregar_arista(g, 4, 5);


    // 3. Ejecutar Pruebas

    printf("\n--- Test de Conectividad Directa ---\n");
    testear_camino(g, 0, 1, 1); // Directo
    testear_camino(g, 4, 5, 1); // Directo en la isla

    printf("\n--- Test de Conectividad Indirecta (Transitiva) ---\n");
    testear_camino(g, 0, 2, 1); // 0->1->2
    testear_camino(g, 0, 3, 1); // 0->1->3 (El más difícil)
    
    printf("\n--- Test de Ciclos (No debe colgarse) ---\n");
    testear_camino(g, 2, 1, 1); // 2->0->1 (Da la vuelta)

    printf("\n--- Test de Casos Falsos (No hay camino) ---\n");
    testear_camino(g, 0, 4, 0); // 0 está en un grupo, 4 en otro
    testear_camino(g, 3, 0, 0); // Grafo DIRIGIDO: 3 no tiene flechas de salida
    testear_camino(g, 5, 4, 0); // 5 no vuelve a 4

    printf("\n--- Test de Extremos (Mismo nodo) ---\n");
    testear_camino(g, 0, 0, 1); // Siempre se llega a uno mismo (trivial)

    // 4. Limpieza

    printf("\n=== FIN DEL TEST ===\n");

    return 0;
}