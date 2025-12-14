#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void *(*FuncionCopiadora)(void *dato);
typedef int (*FuncionComparadora)(void *, void *);
typedef void (*FuncionDestructora)(void *dato);
typedef void (*FuncionVisitanteExtra)(void *dato, void *extra);

typedef enum {
  AVL_RECORRIDO_IN,  /** Inorden */
  AVL_RECORRIDO_PRE, /** Preorden */
  AVL_RECORRIDO_POST /** Postorden */
} AVLRecorrido;


// Estructura de AVL --------------
typedef struct _AVL_Nodo {
  void* dato;
  struct _AVL_Nodo* izq, * der;
  int altura;
} AVL_Nodo;

struct _AVL {
  AVL_Nodo* raiz;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
};

typedef struct _AVL* AVL;

// TABLA HASH -----------------------
typedef struct {
  AVL raiz;
} CasillaHash;

typedef unsigned (*FuncionHash)(void* dato);

/**
 * Estructura principal que representa la tabla hash.
 */
struct _TablaHash {
  CasillaHash *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

typedef struct _TablaHash* TablaHash;

// FUNCIONES PARA AVL #####################################

AVL avl_crear(FuncionCopiadora copia, FuncionComparadora comp,
  FuncionDestructora destr) {
  AVL arbol = malloc(sizeof(struct _AVL));
  assert(arbol != NULL);
  arbol->raiz = NULL;
  arbol->copia = copia;
  arbol->comp = comp;
  arbol->destr = destr;
  return arbol;
}

/**
 * avl_destruir: Destruye el arbol y sus datos.
 */
static void avl_nodo_destruir(AVL_Nodo* raiz, FuncionDestructora destr) {
  if (raiz != NULL) {
    // destruir los nodos en postorder
    avl_nodo_destruir(raiz->izq, destr);
    avl_nodo_destruir(raiz->der, destr);
    destr(raiz->dato);
    free(raiz);
  }
}
void avl_destruir(AVL arbol) {
  avl_nodo_destruir(arbol->raiz, arbol->destr);
  free(arbol);
}

/**
 * avl_buscar: Retorna 1 si el dato se encuentra y 0 en caso contrario
 */
static int avl_nodo_buscar(AVL_Nodo* raiz, FuncionComparadora comp,
  void* dato) {
  if (raiz == NULL)
    return 0;
  else if (comp(dato, raiz->dato) == 0) // raiz->dato == dato
    return 1;
  else if (comp(dato, raiz->dato) < 0) // dato < raiz->dato
    return avl_nodo_buscar(raiz->izq, comp, dato);
  else // raiz->dato < dato
    return avl_nodo_buscar(raiz->der, comp, dato);
}
int avl_buscar(AVL arbol, void* dato) {
  return avl_nodo_buscar(arbol->raiz, arbol->comp, dato);
}

/**
 * avl_nodo_altura: Funcion interna que retorna la altura del arbol.
 * La altura del arbol vacio se define como -1.
 */
static int avl_nodo_altura(AVL_Nodo* raiz) {
  return (raiz == NULL ? -1 : raiz->altura);
}

/**
 * avl_nodo_max_altura_hijos: Funcion interna que retorna la maxima altura de
 * los hijos.
 */
static unsigned int avl_nodo_max_altura_hijos(AVL_Nodo* raiz) {
  assert(raiz != NULL);
  int alturaIzq = avl_nodo_altura(raiz->izq);
  int alturaDer = avl_nodo_altura(raiz->der);
  return (alturaIzq < alturaDer ? alturaDer : alturaIzq);
}

/**
 * avl_nodo_factor_balance: Funcion interna que retorna el factor de balance de
 * un nodo.
 */
static int avl_nodo_factor_balance(AVL_Nodo* raiz) {
  assert(raiz != NULL);
  int factor = avl_nodo_altura(raiz->der) - avl_nodo_altura(raiz->izq);
  assert(-2 <= factor || factor <= 2);
  return factor;
}

/**
 * avl_nodo_rotacion_simple_izq: Funcion interna que realiza una rotacion simple
 * a izquierda y retorna la nueva raiz.
 */
static AVL_Nodo* avl_nodo_rotacion_simple_izq(AVL_Nodo* raiz) {
  AVL_Nodo* hijoDer = raiz->der;
  assert(hijoDer != NULL);
  // actualizar punteros
  raiz->der = hijoDer->izq;
  hijoDer->izq = raiz;
  // actualizar alturas
  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
  hijoDer->altura = 1 + avl_nodo_max_altura_hijos(hijoDer);
  return hijoDer;
}

/**
 * avl_nodo_rotacion_simple_der: Funcion interna que realiza una rotacion simple
 * a derecha y retorna la nueva raiz.
 */
static AVL_Nodo* avl_nodo_rotacion_simple_der(AVL_Nodo* raiz) {
  AVL_Nodo* hijoizq = raiz->izq;
  assert(hijoizq != NULL);
  raiz->izq = hijoizq->der;
  hijoizq->der = raiz;

  raiz->altura = 1+ avl_nodo_max_altura_hijos(raiz);
  hijoizq->altura = 1 + avl_nodo_max_altura_hijos(hijoizq);
  
  return hijoizq;
}

/**
 * avl_nodo_crear: Funcion interna que crea un nuevo nodo y lo retorna.
 * La altura de un nodo hoja es 0.
 */
static AVL_Nodo* avl_nodo_crear(void* dato, FuncionCopiadora copia) {
  AVL_Nodo* nuevoNodo = malloc(sizeof(AVL_Nodo));
  assert(nuevoNodo != NULL);
  nuevoNodo->dato = copia(dato);
  nuevoNodo->izq = nuevoNodo->der = NULL;
  nuevoNodo->altura = 0;
  return nuevoNodo;
}

/**
 * avl_insertar: Inserta un dato no repetido en el arbol, manteniendo la
 * propiedad de los arboles AVL.
 */
static AVL_Nodo* avl_nodo_insertar(AVL_Nodo* raiz, void* dato,
  FuncionCopiadora copia, FuncionComparadora comp) {
  if (raiz == NULL) // insertamos el nuevo elemento
    return avl_nodo_crear(dato, copia);
  else if (comp(dato, raiz->dato) < 0) { // el dato debe ir en el subarbol izq
    raiz->izq = avl_nodo_insertar(raiz->izq, dato, copia, comp);
    // chequear balance
    if (avl_nodo_factor_balance(raiz) == -2) {
      // casos 1 o 2
      if (avl_nodo_factor_balance(raiz->izq) == 1) raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
      raiz = avl_nodo_rotacion_simple_der(raiz); // caso 1
    }
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    return raiz;
  }
  else if (comp(raiz->dato, dato) < 0) { // el dato debe ir en el subarbol der
    /** COMPLETAR */
    raiz->der = avl_nodo_insertar(raiz->der, dato, copia, comp);
    
    if(avl_nodo_factor_balance(raiz) == 2){
        if(avl_nodo_factor_balance(raiz->der) == -1) raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
        raiz = avl_nodo_rotacion_simple_izq(raiz);
    }

    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    return raiz;

  }
  else // no agregar elementos repetidos
    return raiz;
}
void avl_insertar(AVL arbol, void* dato) {
  arbol->raiz = avl_nodo_insertar(arbol->raiz, dato, arbol->copia, arbol->comp);
}

/**
 * avl_validar: Retorna 1 si el arbol cumple la propiedad de los arboles AVL,
 * y 0 en caso contrario.
 * avl_nodo_validar_abb: Funcion interna que retorna 1 si el arbol cumple la
 * propiedad de los arboles BB, y 0 en caso contrario.
 * avl_nodo_validar_altura_y_balance: Funcion interna que retorna 1 si la
 * altura y el factor de balance de los nodos son correctos, y 0 en caso
 * contrario.
 */
static int avl_nodo_validar_abb(AVL_Nodo* raiz, void* min, void* max,
  FuncionComparadora comp) {
  // si la raiz es vacia, retornar exitosamente
  if (raiz == NULL)
    return 1;
  else {
    // sino, validar intervalo
    if (min != NULL && comp(raiz->dato, min) <= 0)
      return 0;
    if (max != NULL && comp(max, raiz->dato) <= 0)
      return 0;
    // y validar subarboles recursivamente
    return (avl_nodo_validar_abb(raiz->izq, min, raiz->dato, comp) &&
      avl_nodo_validar_abb(raiz->der, raiz->dato, max, comp));
  }
}
static int avl_nodo_validar_altura_y_balance(AVL_Nodo* raiz) {
  // si la raiz es vacia, retornar exitosamente
  if (raiz == NULL)
    return 1;
  // sino, validar subarboles recursivamente
  int ret1 = avl_nodo_validar_altura_y_balance(raiz->izq);
  int ret2 = avl_nodo_validar_altura_y_balance(raiz->der);
  if (ret1 && ret2) {
    // si ambos subarboles son validos, validar altura y balance de raiz
    int altura = 1 + avl_nodo_max_altura_hijos(raiz);
    int balance = avl_nodo_factor_balance(raiz);
    if ((raiz->altura == altura) && (balance >= -1) && (balance <= 1))
      return 1;
  }
  // en cualquier otro caso, retornar falso
  return 0;
}
int avl_validar(AVL arbol) {
  return (avl_nodo_validar_altura_y_balance(arbol->raiz) &&
    avl_nodo_validar_abb(arbol->raiz, NULL, NULL, arbol->comp));
}

/**
 * avl_recorrer: Recorrido DSF del arbol
 */
static void avl_nodo_recorrer(AVL_Nodo* raiz, AVLRecorrido orden,
  FuncionVisitanteExtra visita, void* extra) {
  if (raiz != NULL) {
    if (orden == AVL_RECORRIDO_PRE)
      visita(raiz->dato, extra);
    avl_nodo_recorrer(raiz->izq, orden, visita, extra);
    if (orden == AVL_RECORRIDO_IN)
      visita(raiz->dato, extra);
    avl_nodo_recorrer(raiz->der, orden, visita, extra);
    if (orden == AVL_RECORRIDO_POST)
      visita(raiz->dato, extra);
  }
}
void avl_recorrer(AVL arbol, AVLRecorrido orden, FuncionVisitanteExtra visita,
  void* extra) {
  avl_nodo_recorrer(arbol->raiz, orden, visita, extra);
}

static AVL_Nodo* rebalancear(AVL_Nodo* raiz){
    if (avl_nodo_factor_balance(raiz) == 2)
    {
        if(avl_nodo_factor_balance(raiz->der) == -1) raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
        raiz = avl_nodo_rotacion_simple_izq(raiz);
    }
    else if(avl_nodo_factor_balance(raiz) == -2){
        if(avl_nodo_factor_balance(raiz->izq) == 1) raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
        raiz = avl_nodo_rotacion_simple_der(raiz);
    }
    else return raiz;
    return raiz;   
}

static AVL_Nodo* avl_eliminar(AVL_Nodo* raiz, void* dato,FuncionComparadora comp, FuncionDestructora destroy){
    if (raiz == NULL) return NULL;
    int c = comp(dato, raiz->dato);
    if (c<0) return avl_eliminar(raiz->izq, dato, comp, destroy);
    else if(c>0) return avl_eliminar(raiz->der, dato, comp, destroy);
    else{
        if(raiz->der == NULL || raiz->izq == NULL){
            AVL_Nodo* sucesor = raiz->der == NULL ? raiz->izq : raiz->der;
            destroy(raiz->dato);
            free(raiz);
            return sucesor;
        }
        else{
            AVL_Nodo* sucesor = raiz->der;
            while(raiz->izq != NULL){
                sucesor = sucesor->izq;
            }
            void* copydato = raiz->dato;
            raiz->dato = sucesor->dato;
            sucesor->dato = copydato;

            raiz = avl_eliminar(raiz, sucesor->dato, comp, destroy);

        }
    }
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    
    return rebalancear(raiz);
}

// FUNCIONES PARA AVL ###################################################################


// #########################################################################################################


// FUNCIONES PARA HASH ###################################################################
/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash) {

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(CasillaHash) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->destr = destr;
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->elems[idx].raiz = avl_crear(copia, comp, destr);
  }

  return tabla;
}

/**
 * Retorna el numero de elementos de la tabla.
 */
int tablahash_nelems(TablaHash tabla) { return tabla->numElems; }

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla) { return tabla->capacidad; }

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla) {

  // Destruir cada uno de los datos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx)
    if (tabla->elems[idx].raiz != NULL)
      avl_destruir(tabla->elems[idx].raiz);

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}


void avl_idx_nodo(AVL_Nodo* raiz, TablaHash tabla, CasillaHash* nueva, int nuevacapacidad){
    if(raiz == NULL) return;
    unsigned idx = tabla->hash(raiz->dato) % nuevacapacidad;
    avl_insertar(nueva[idx].raiz, raiz->dato);
    avl_idx_nodo(raiz->izq, tabla,nueva, nuevacapacidad);
    avl_idx_nodo(raiz->der, tabla,nueva, nuevacapacidad);
    
}

void avl_idx(AVL avl, TablaHash tabla, CasillaHash* nueva, int nuevacap){
    avl_idx_nodo(avl->raiz, tabla, nueva, nuevacap);
}

float factor_de_carga(TablaHash tabla){
    return (float)tabla->numElems / tabla->capacidad;
}


void rehashear(TablaHash tabla){
    // Lo ideal es escoger el proximo primo como nueva capacidad pero elegire el doble del tamaño ya que no se como calcular
    // el siguiente numero primo
    int nuevacap = tabla->capacidad * 2;
    CasillaHash* nuevo = malloc(sizeof(CasillaHash)*nuevacap);
    for(int i = 0; i<nuevacap; i++){
        nuevo[i].raiz = avl_crear(tabla->copia, tabla->comp, tabla->destr);
    }
    for(int i = 0; i < tabla->capacidad; i++){
        if(tabla->elems[i].raiz != NULL){
            avl_idx(tabla->elems[i].raiz, tabla , nuevo, nuevacap);
        }
    }
    for(int i = 0; i<tabla->capacidad; i++){
        if(tabla->elems[i].raiz != NULL) avl_destruir(tabla->elems[i].raiz);     
    }
    free(tabla->elems);
    tabla->elems = nuevo;
    tabla->capacidad = nuevacap;

}


/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */
void tablahash_insertar(TablaHash tabla, void *dato) {
    if(factor_de_carga(tabla) > 0.80){
        rehashear(tabla);
    }

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Insertar el dato si la casilla estaba libre.
  if (tabla->elems[idx].raiz == NULL) {
    tabla->numElems++;
    avl_insertar(tabla->elems[idx].raiz, dato); // Mi insertar rebalancea
    return;
  }
  // Si el dato ya estaba en la tabla no hacer nada
  else if (avl_buscar(tabla->elems[idx].raiz, dato) == 1) {
    return;
  }
  // Insertar si hay colision
  else {
    tabla->numElems++;
    avl_insertar(tabla->elems[idx].raiz, dato);
    return;
  }
}



void* avl_nodo_buscar_retornar(AVL_Nodo* raiz, FuncionComparadora comp,
  void* dato) {
  if (raiz == NULL)
    return NULL;
  else if (comp(dato, raiz->dato) == 0) // raiz->dato == dato
    return raiz->dato;
  else if (comp(dato, raiz->dato) < 0) // dato < raiz->dato
    return avl_nodo_buscar_retornar(raiz->izq, comp, dato);
  else // raiz->dato < dato
    return avl_nodo_buscar_retornar(raiz->der, comp, dato);
}
void* avl_buscar_retornar(AVL arbol, void* dato) {
  return avl_nodo_buscar_retornar(arbol->raiz, arbol->comp, dato);
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void* tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Retornar NULL si la casilla estaba vacia.
  if (tabla->elems[idx].raiz == NULL)
    return NULL;
  // Retornar el dato de la casilla si hay concidencia.
  else if (avl_buscar(tabla->elems[idx].raiz, dato) == 1)
    return avl_buscar_retornar(tabla->elems[idx].raiz, dato);
  // Retornar NULL en otro caso.
  else
    return NULL;
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Retornar si la casilla estaba vacia.
  if (tabla->elems[idx].raiz == NULL)
    return;
  // Vaciar la casilla si hay coincidencia.
  else if (avl_buscar(tabla->elems[idx].raiz,dato)) {
    tabla->numElems--;
    tabla->elems[idx].raiz->raiz = avl_eliminar(tabla->elems[idx].raiz->raiz, dato, tabla->elems[idx].raiz->comp, tabla->elems[idx].raiz->destr);
    return;
  }
}


// Voy a usar datos tipo int para probar mi implementacion
void* copiar_entero(void* dato1){
    int* value = malloc(sizeof(int));
    assert(value != NULL);
    *value = *(int*)dato1;
    return value;
}

void destruir_entero(void* dato){
    free(dato);
}

int comparar_entero(void* dato1, void* dato2){
    return *(int*)dato1 - *(int*)dato2;
}

unsigned hashear_entero(void* dato){
    return *(unsigned*)dato;
}

void imprimir_entero(void* dato, void*extra){
    printf("%d ", *(int*)dato);
}

int main(){
    TablaHash tablita = tablahash_crear(5, copiar_entero, comparar_entero, destruir_entero, hashear_entero);

    for(int i = 0; i < 10; i++){
        tablahash_insertar(tablita, &i);
    }

    assert(tablita->numElems == 10);
    assert(tablita->capacidad > 5);

    printf("La cantidad de elementos despues de la primera insercion es: %d.\n", tablita->numElems);
    printf("La capacidad de la hash es: %d\n", tablita->capacidad);
    /*
    Para ver si maneja bien las colisiones, hay que comentar el rehasheo sino no se puede visualizar
    avl_recorrer(tablita->elems[0].raiz, AVL_RECORRIDO_IN, imprimir_entero, NULL);
    */

    printf("\n--- Prueba de Duplicados ---\n");
    int duplicado = 5;
    tablahash_insertar(tablita, &duplicado);
    // La cantidad NO debe cambiar
    if (tablahash_nelems(tablita) == 10) printf("Check: Duplicados ignorados correctamente.\n");
    else printf("ERROR: Se insertó un duplicado.\n");
    assert(tablahash_nelems(tablita) == 10);
    puts("");

    printf("\n--- Forzando Colisiones en indice 0 ---\n");
    int colisiones[] = {10, 20, 30, 40}; 
    for(int i=0; i<4; i++) tablahash_insertar(tablita, &colisiones[i]);
    
    // Imprimimos solo el índice 0 para ver si se formó el AVL
    printf("Estructura del AVL en indice 0 (deberia tener varios nodos):\n");
    if(tablita->elems[0].raiz->raiz != NULL){
        avl_recorrer(tablita->elems[0].raiz, AVL_RECORRIDO_PRE, imprimir_entero, NULL);
    }
    printf("\n");

    puts("");  
    int t = 3;
    void* datoEliminar = tablahash_buscar(tablita, &t);
    if(datoEliminar == NULL) {
        printf("No se encontro el dato\n");
    } else printf("%d\n", *(int*)datoEliminar);

    for(int i = 3; i < 5; i++){
        tablahash_eliminar(tablita, &i);
    }

    puts("");  
    void* datoEliminado = tablahash_buscar(tablita, &t);     
    if(datoEliminado == NULL){
        printf("No se encontro el dato\n");
    } else printf("%d\n", *(int*)datoEliminado);
    
    printf("La cantidad de elementos despues de la eliminacion es: %d.\n", tablita->numElems);
    

    // 6. ELIMINACION DE NO EXISTENTE
    int fantasma = 999;
    int cant_antes = tablahash_nelems(tablita);
    tablahash_eliminar(tablita, &fantasma);
    assert(tablahash_nelems(tablita) == cant_antes); // No debe cambiar nada
    printf("Check: Eliminar elemento inexistente no rompió nada.\n");

    // 7. DESTRUCCION
    printf("\n--- Destruyendo Tabla ---\n");
    tablahash_destruir(tablita);
    printf("=== FIN DE TEST (Sin errores de memoria) ===\n");

    return 0;
}
