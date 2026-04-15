#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopiadora)(void *dato);
typedef void (*FuncionVisitante)(void *dato);
typedef int (*FuncionComparadora)(void *dato1, void *dato2);
typedef int (*Predicado) (void *dato);
typedef unsigned (*FuncionHash)(void *dato);

#define UMBRAL 8

// --------------------------------------------------------------------------- GLIST
typedef struct _GNode {
  void *data;
  struct _GNode *next;
} GNode;

typedef struct _GNode *GList;
/**
 * Devuelve una lista vacía.
 */
GList glist_crear() { 
  return NULL; 
}

/**
 * Destruccion de la lista.
 */
void glist_destruir(GList list, FuncionDestructora destroy) {
  GNode *nodeToDelete;
  while (list != NULL) {
    nodeToDelete = list;
    list = list->next;
    destroy(nodeToDelete->data); 
    free(nodeToDelete);
  }
}

/**
 * Determina si la lista es vacía.
 */
int glist_vacia(GList list) { 
  return (list == NULL); 
}

/**
 * Agrega un elemento al inicio de la lista.
 */
GList glist_agregar_inicio(GList list, void *data, FuncionCopiadora copy) {
  GNode *newNode = malloc(sizeof(GNode));
  assert(newNode != NULL);
  newNode->next = list;
  newNode->data = copy(data);
  return newNode;
}

/**
 * Busca un elemento en la lista.
 */
int glist_buscar(GList list, void* data, FuncionComparadora comp){
  int found = 0;
  for(GNode *node = list; found == 0 && node != NULL; node = node->next)
    if (comp(node->data, data) == 0) found = 1;
  return found;
}

void* glist_buscar_dato(GList list, void* data, FuncionComparadora comp){
  void* found = NULL;
  for(GNode *node = list; found == NULL && node != NULL; node = node->next)
    if (comp(node->data, data) == 0) found = node->data;
  return found;
}

GNode* glist_buscar_nodo(GList lista, void* dato, FuncionComparadora cmp){
    GNode* found = NULL;
    for(GNode* nodo = lista; found == NULL && nodo != NULL; nodo = nodo->next)
        if(cmp(nodo->data, dato) == 0) found = nodo;
    return found;
}

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void glist_recorrer(GList list, FuncionVisitante visit) {
  for (GNode *node = list; node != NULL; node = node->next)
    visit(node->data);
}

GList glist_eliminar(GList lista, void* dato, FuncionDestructora destroy, FuncionComparadora cmp){
    GNode* temp = lista;
    GNode* ant = NULL;
    while(temp != NULL && cmp(dato, temp->data) != 0){
        temp = temp->next;
    }
    if(temp == NULL) return lista;
    if(ant == NULL){
        lista = temp->next;
    }
    else{
        ant->next = temp->next;
    }
    destroy(temp->data);
    free(temp);
    return lista;
}   

// --------------------------------------------------------------------------- AVL

typedef enum {
  AVL_RECORRIDO_IN,  /** Inorden */
  AVL_RECORRIDO_PRE, /** Preorden */
  AVL_RECORRIDO_POST /** Postorden */
} AVLRecorrido;

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

typedef struct _AVL *AVL;


/**
 * avl_crear: Retorna un arbol AVL vacio
 */
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

static void* avl_nodo_buscar_dato(AVL_Nodo* raiz, FuncionComparadora comp,
  void* dato) {
  if (raiz == NULL)
    return NULL;
  else if (comp(dato, raiz->dato) == 0) // raiz->dato == dato
    return raiz->dato;
  else if (comp(dato, raiz->dato) < 0) // dato < raiz->dato
    return avl_nodo_buscar_dato(raiz->izq, comp, dato);
  else // raiz->dato < dato
    return avl_nodo_buscar_dato(raiz->der, comp, dato);
}
void* avl_buscar_dato(AVL arbol, void* dato) {
  return avl_nodo_buscar_dato(arbol->raiz, arbol->comp, dato);
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
    AVL_Nodo* hijoIzq = raiz->izq;
    assert(hijoIzq != NULL);
    // actualizar punteros
    raiz->izq = hijoIzq->der;
    hijoIzq->der = raiz;
    // actualizar alturas
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    hijoIzq->altura = 1 + avl_nodo_max_altura_hijos(hijoIzq);
    return hijoIzq;
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

AVL_Nodo* rebalance(AVL_Nodo* root){
    if(avl_nodo_factor_balance(root) == 2){
        if(avl_nodo_factor_balance(root) == -1) root->der = avl_nodo_rotacion_simple_der(root->der);
        root = avl_nodo_rotacion_simple_izq(root);
    }
    else if(avl_nodo_factor_balance(root) == -2){
        if(avl_nodo_factor_balance(root->izq) == 1) root->izq = avl_nodo_rotacion_simple_izq(root->izq);
        root = avl_nodo_rotacion_simple_der(root);
    }
    else return root;
    return root;
}

/**
 * avl_insertar: Inserta un dato no repetido en el arbol, manteniendo la
 * propiedad de los arboles AVL.
 */
static AVL_Nodo* avl_nodo_insertar(AVL_Nodo* raiz, void* dato,
  FuncionCopiadora copia, FuncionComparadora comp, int* inserto) {
  if (raiz == NULL) // insertamos el nuevo elemento
    return avl_nodo_crear(dato, copia);
  else if (comp(dato, raiz->dato) < 0) { // el dato debe ir en el subarbol izq
    raiz->izq = avl_nodo_insertar(raiz->izq, dato, copia, comp, inserto);
    // chequear balance
    if (avl_nodo_factor_balance(raiz) == -2) {
      // casos 1 o 2
      if (avl_nodo_factor_balance(raiz->izq) == 1) // caso 2
        raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
      raiz = avl_nodo_rotacion_simple_der(raiz); // caso 1
    }
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    return raiz;
  }
  else if (comp(raiz->dato, dato) < 0) { // el dato debe ir en el subarbol der
    raiz->der = avl_nodo_insertar(raiz->der, dato, copia, comp, inserto);
    if(avl_nodo_factor_balance(raiz) == 2){
        if(avl_nodo_factor_balance(raiz->der) == -1) raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
        raiz = avl_nodo_rotacion_simple_izq(raiz);
    }
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    return raiz;
  }
  else // no agregar elementos repetidos
    *inserto = 0;
    return raiz;
}
void avl_insertar(AVL arbol, void* dato, int* inserto) {
  arbol->raiz = avl_nodo_insertar(arbol->raiz, dato, arbol->copia, arbol->comp, inserto);
}

AVL_Nodo* avl_nodo_eliminar(AVL_Nodo* root, void* dato, FuncionComparadora cmp, FuncionDestructora dest){
    if(root == NULL) return NULL;
    int c = cmp(dato, root->dato);
    if(c < 0){
        root->izq = avl_nodo_eliminar(root->izq, dato, cmp, dest);
    }
    else if(c > 0){
        root->der = avl_nodo_eliminar(root->der, dato, cmp, dest);
    }
    else{
        if(!root->izq || !root->der){
            AVL_Nodo* sucesor = root->der == NULL ? root->izq : root->der;
            dest(root->dato);
            free(root);
            return sucesor;
        }
        else{
            AVL_Nodo* sucesor = root->der;
            while(sucesor->izq != NULL) sucesor = sucesor->izq;
            void* copia_dato = root->dato;
            root->dato = sucesor->dato;
            sucesor->dato = copia_dato;
            root->der = avl_nodo_eliminar(root->der, sucesor->dato, cmp, dest);
            root->altura = 1 + avl_nodo_max_altura_hijos(root);
            return rebalance(root);
        }
    }
    root->altura = 1 + avl_nodo_max_altura_hijos(root);
    return rebalance(root);
}

void avl_eliminar(AVL tree, void* dato){
  tree->raiz = avl_nodo_eliminar(tree->raiz, dato, tree->comp, tree->destr);
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
  FuncionVisitante visita) {
  if (raiz != NULL) {
    if (orden == AVL_RECORRIDO_PRE)
      visita(raiz->dato);
    avl_nodo_recorrer(raiz->izq, orden, visita);
    if (orden == AVL_RECORRIDO_IN)
      visita(raiz->dato);
    avl_nodo_recorrer(raiz->der, orden, visita);
    if (orden == AVL_RECORRIDO_POST)
      visita(raiz->dato);
  }
}
void avl_recorrer(AVL arbol, AVLRecorrido orden, FuncionVisitante visita) {
  avl_nodo_recorrer(arbol->raiz, orden, visita);
}

// --------------------------------------------------------------------------- TABLA HASH
/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef enum{
    ARBOL,
    LISTA,
}Estado;

typedef struct {
  void* nodo;
  Estado estadoCasilla;
  int cantidad;
} CasillaHash;

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
typedef struct _TablaHash *TablaHash;

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
    tabla->elems[idx].nodo = NULL;;
    tabla->elems[idx].cantidad = 0;
    tabla->elems[idx].estadoCasilla = LISTA;
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
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx){
        if (tabla->elems[idx].estadoCasilla == LISTA)
            glist_destruir((GList)tabla->elems[idx].nodo, tabla->destr);
        if(tabla->elems[idx].estadoCasilla == ARBOL)
            avl_destruir((AVL)tabla->elems[idx].nodo);
  }

   

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}

AVL glist_to_avl(GList lista, FuncionCopiadora copy, FuncionComparadora cmp, FuncionDestructora dest){
    AVL arbol = avl_crear(copy, cmp, dest);
    GNode* temp = lista;
    int inserto = 1;
    while(temp != NULL){
        avl_insertar(arbol, temp->data, &inserto); temp = temp->next;
    }
    glist_destruir(lista, dest);
    return arbol;
}


/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */
void tablahash_insertar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  // Insertar el dato si la casilla estaba libre.
  if (tabla->elems[idx].nodo == NULL) {
    tabla->elems[idx].nodo = glist_agregar_inicio((GList)tabla->elems[idx].nodo, dato, tabla->copia);
    tabla->elems[idx].cantidad++;
    tabla->numElems++;
    return;
  }
  // Sobrescribir el dato si el mismo ya se encontraba en la tabla.
  else if (tabla->elems[idx].estadoCasilla == LISTA && tabla->elems[idx].cantidad < UMBRAL) {
    GNode* encontrado = glist_buscar_nodo((GList)tabla->elems[idx].nodo, dato, tabla->comp);
    if(encontrado){
        tabla->destr(encontrado->data);
        encontrado->data = tabla->copia(dato);
        return;
    }
    else{
        tabla->elems[idx].nodo = glist_agregar_inicio((GList)tabla->elems[idx].nodo, dato, tabla->copia);
        tabla->elems[idx].cantidad++;
        tabla->numElems++;
        return;
    }

  }
  else if(tabla->elems[idx].cantidad == UMBRAL){
    tabla->elems[idx].nodo = glist_to_avl((GList)tabla->elems[idx].nodo, tabla->copia,tabla->comp, tabla->destr);
    int inserto = 1;
    avl_insertar((AVL)tabla->elems[idx].nodo, dato, &inserto);
    if(inserto == 1){
      tabla->numElems++; tabla->elems[idx].cantidad++; return;
    }
    else return;
  }
  else if(tabla->elems[idx].estadoCasilla == ARBOL){
    int inserto = 1;
    avl_insertar((AVL)tabla->elems[idx].nodo, dato, &inserto);
    if(inserto == 1){
      tabla->numElems++; tabla->elems[idx].cantidad++; return;
    }
    else return;
  }
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Retornar NULL si la casilla estaba vacia.
  if (tabla->elems[idx].estadoCasilla == LISTA)
    return glist_buscar_dato((GList)tabla->elems[idx].nodo, dato, tabla->comp);
  // Retornar el dato de la casilla si hay concidencia.
  else if (tabla->elems[idx].estadoCasilla == ARBOL)
    return avl_buscar_dato((AVL)tabla->elems[idx].nodo, dato);
  // Retornar NULL en otro caso.
  else
    return NULL;
}

void* no_copy(void* dato){
  return dato;
}

void no_destroy(void* dato){
  return;
}

static void avl_nodo_to_list(AVL_Nodo* raiz, GList* lista){
  if(raiz == NULL) return;
  avl_nodo_to_list(raiz->izq, lista);
  *lista = glist_agregar_inicio(*lista, raiz->dato, no_copy);
  avl_nodo_to_list(raiz->der, lista);
}
void avl_to_list(AVL tree, GList* lista){
  avl_nodo_to_list(tree->raiz, lista);
  avl_nodo_destruir(tree->raiz, no_destroy);
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  if(tabla->elems[idx].estadoCasilla == LISTA){
    tabla->elems[idx].nodo = glist_eliminar((GList)tabla->elems[idx].nodo, dato, tabla->destr, tabla->comp);
    tabla->elems[idx].cantidad--; tabla->numElems--;
    return;
  }
  if(tabla->elems[idx].estadoCasilla == ARBOL){
    if(tabla->elems[idx].cantidad >= UMBRAL/2){
      avl_eliminar((AVL)tabla->elems[idx].nodo, dato);
      tabla->elems[idx].cantidad--; tabla->numElems--;
      return;
    }
    else{
      GList lista = glist_crear();
      avl_eliminar((AVL)tabla->elems[idx].nodo, dato);
      avl_to_list((AVL)tabla->elems[idx].nodo, &lista);
      free(tabla->elems[idx].nodo);
      tabla->elems[idx].nodo = lista;
      tabla->elems[idx].cantidad--; tabla->elems[idx].estadoCasilla = LISTA;
      tabla->numElems--;
      return;
    }
  }
}

void recorrerHash(TablaHash hash, FuncionVisitante visit){
    for(int i = 0; i<hash->capacidad; i++){
        if(hash->elems[i].estadoCasilla == LISTA)
            glist_recorrer((GList)hash->elems[i].nodo, visit);
        else if(hash->elems[i].estadoCasilla == ARBOL)
            avl_recorrer((AVL)hash->elems[i].nodo, AVL_RECORRIDO_IN, visit);
    }
    return;
}

void destruir_entero(void* dato){
    free(dato);
}
void* copiar_entero(void* dato){
    int* val = malloc(sizeof(int));
    assert(val);
    *val = *(int*)dato;
    return val;
}

void imprimir_entero(void* dato){
    printf("%d ", *(int*)dato);
}
int comparar_entero(void* dato1, void* dato2){
    return *(int*)dato1 - *(int*)dato2;
}

unsigned hashear_entero(void* dato){
    return *(int*)dato;
}

int main(){
    TablaHash tabla = tablahash_crear(5, copiar_entero, comparar_entero, destruir_entero, hashear_entero);
    for(int i = 0; i < 100; i++){
        tablahash_insertar(tabla, &i);
    }
    for(int i = 30; i <= 90; i++){
      tablahash_eliminar(tabla, &i);
    }

    int eliminar[5] = {15, 20, 95, 25,10};
    for(int i = 0; i<5; i++) tablahash_eliminar(tabla, &eliminar[i]);

    recorrerHash(tabla, imprimir_entero);
    puts("");
    printf("LA CANTIDAD DE ELEMENTOS DE LA TABLA ES DE %d\n", tabla->numElems);
    for(int i = 0; i<tabla->capacidad;i++){
      printf("La casilla %d tiene %d elementos, y es un %d (0: AVL, 1: Lista)\n", i,tabla->elems[i].cantidad, 
        tabla->elems[i].estadoCasilla);
    }
    
    if(tabla->elems[0].estadoCasilla == ARBOL) avl_recorrer((AVL)tabla->elems[0].nodo, AVL_RECORRIDO_IN, imprimir_entero);
    else glist_recorrer((GList)tabla->elems[0].nodo, imprimir_entero);
    puts("");
    
    


    tablahash_destruir(tabla);
    return 0;
}