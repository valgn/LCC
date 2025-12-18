#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void *(*FuncionCopiadora)(void *dato);
typedef int (*FuncionComparadora)(void *, void *);
typedef void (*FuncionDestructora)(void *dato);
typedef void (*FuncionVisitanteExtra)(void *dato, void *extra);

typedef struct _Anodo{
    void* x;
    struct _Anodo* der, *izq;
}Anodo;

typedef Anodo* GRUPO;

GRUPO crear_arbol(){ return NULL; }

/**
 * bstree_destruir: Destruye el arbol y sus datos
 */
void bstree_destruir(GRUPO raiz, FuncionDestructora destr) {
  if (raiz != NULL) {
    bstree_destruir(raiz->izq, destr);
    bstree_destruir(raiz->der, destr);
    destr(raiz->x);
    free(raiz);
  }
}

/**
 * bstree_buscar: Retorna 1 si el dato se encuentra y 0 en caso
 * contrario
 */
int pertenece(GRUPO raiz, void *dato, FuncionComparadora comp) {
  if (raiz == NULL)
    return 0;
  else if (comp(dato, raiz->x) == 0) // raiz->dato == dato
    return 1;
  else if (comp(dato, raiz->x) < 0) // dato < raiz->dato
    return pertenece(raiz->izq, dato, comp);
  else // raiz->dato < dato
    return pertenece(raiz->der, dato, comp);
}

/**
 * bstree_insertar: Inserta un dato no repetido en el arbol, manteniendo la
 * propiedad del arbol de busqueda binaria
 */
GRUPO grupo_insertar(GRUPO raiz, void *dato, FuncionCopiadora copia,
                       FuncionComparadora comp) {
  if (raiz == NULL) { // insertar el dato en un nuevo nodo
    struct _Anodo *nuevoNodo = malloc(sizeof(struct _Anodo));
    assert(nuevoNodo != NULL);
    nuevoNodo->x = copia(dato);
    nuevoNodo->izq = nuevoNodo->der = NULL;
    return nuevoNodo;
  } else if (comp(dato, raiz->x) < 0) // dato < raiz->dato
    raiz->izq = grupo_insertar(raiz->izq, dato, copia, comp);
  else if (comp(dato, raiz->x) > 0) // raiz->dato < dato
    raiz->der = grupo_insertar(raiz->der, dato, copia, comp);
  // si el dato ya se encontraba, no es insertado
  return raiz;
}


/*4. Implemente una funci´on es grupo que dado un conjunto, un elemento de dicho conjunto que es
candidato a elemento neutro y una operaci´on de enteros determine si el conjunto dotado de la operaci´on
es un grupo.
*/
typedef int (*OperacionLoca)(int d1, int d2);

int es_neutro(GRUPO set, int e, OperacionLoca f){
    if(set == NULL) return 1;
    if(f(*(int*)set->x, e) != *(int*)set->x || f(e, *(int*)set->x) != *(int*)set->x) return 0;
    return es_neutro(set->izq, e, f) && es_neutro(set->der, e, f);
}

int ver_todo(GRUPO set, int dato ,int e, OperacionLoca f){
    if(set == NULL) return 0;

    if(f(*(int*)set->x, dato) == e && f(dato, *(int*)set->x) == e) return 1;

    return ver_todo(set->der, dato, e, f) || ver_todo(set->izq, dato, e, f);

}


int verificar_inverso(GRUPO set, GRUPO raiz,int e, OperacionLoca f){
    if(set == NULL) return 1;
    int dato = *(int*)set->x;
    int verif = ver_todo(raiz, dato, e, f);
    if(verif == 0) return 0; //Cortamos antes
    return verif && verificar_inverso(set->der, raiz, e, f) && verificar_inverso(set->izq, raiz, e,f);

}

int es_grupo(GRUPO set, void* e, OperacionLoca f){
    int n = *(int*)e;
    int neutro = es_neutro(set, n, f);
    if(!neutro) return 0; //Cortamos antes
    int verif = verificar_inverso(set, set, n, f);
    return verif;
}

void* copiar_entero(void* dato){
    int* val = malloc(sizeof(int));
    *val = *(int*)dato;
    return val;
}

int comparar_entero(void* d1, void* d2){
    return *(int*)d1 - *(int*)d2;
}

void destruir_entero(void* a){
    free(a);
}

int suma_enteros(int a, int b){
    return a+b;
}
typedef void (*FuncionVisitante)(void*);
void imprimir_entero(void* dato){
    printf("%d ", *(int*)dato);
}

void imprimir_set(GRUPO set, FuncionVisitante visit){
    if(set == NULL) return;
    // Imprimir in-order
    imprimir_set(set->izq, visit);
    visit(set->x);
    imprimir_set(set->der, visit);
}

int main(){
    printf("Iniciar test: \n");
    puts("");
    printf("Creamos un grupo vacio...\n");
    GRUPO grupo = crear_arbol();
    assert(grupo == NULL);
    printf("Grupo creado! Insertemos elementos\n");
    for(int i = -9; i < 10; i++){
        grupo = grupo_insertar(grupo, &i,copiar_entero,comparar_entero);
    }
    assert(grupo != NULL);
    printf("Los elementos se insertaron correctamente! Veamos como esta conformado el conjunto\n");
    imprimir_set(grupo, imprimir_entero);
    puts("");

    puts("");
    int cero = 0;
    printf("El %d pertenece al grupo? ", cero);
    int xinG = pertenece(grupo, &cero, comparar_entero);
    assert(xinG == 1); printf("SI.\n");
    printf("Veamos ahora si el conjunto cumple la definicion de grupo. Deberia cumplirlo.\n");
    printf("El grupo es G=([-9,9],0,+)\n");
    int verificacion = es_grupo(grupo, &cero, suma_enteros);
    assert(verificacion == 1); printf("Test pasado correctamente, el conjunto es un grupo.\n");

    printf("Destruyendo el primer grupo.\n");
    bstree_destruir(grupo, destruir_entero);

    printf("Creemos un segundo conjunto que no cumpla la condicion de grupo\n");
    GRUPO nogrupo = crear_arbol();
    for(int i = 1; i < 6; i++){
        nogrupo = grupo_insertar(nogrupo, &i, copiar_entero, comparar_entero);
    }
    printf("Insertamos elementos de forma que el conjunto no tenga inversos ni neutro. Veamos como queda el conjunto\n");
    imprimir_set(nogrupo, imprimir_entero);
    puts("");
    printf("Ahora veamos que definitivamente no es un grupo.\n");
    int neutro = 1;
    int verificacion2 = es_grupo(nogrupo, &neutro, suma_enteros);
    assert(verificacion2 == 0); printf("Paso el test de que no es un grupo!\n");

    bstree_destruir(nogrupo, destruir_entero);
    return 0;
}