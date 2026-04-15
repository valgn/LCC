#include <stdlib.h>
#include <assert.h>

#include "listas.h"

ListaSimple lse_crear(FuncionComparadora comp, FuncionCopiadora copy, FuncionDestructora destr, FuncionVisitante visit){
    ListaSimple lista = malloc(sizeof(struct Simple));
    assert(lista != NULL);
    lista->comp = comp;
    lista->copy = copy;
    lista->destroy = destr;
    lista->visit = visit;
    lista->inicio = NULL;
    lista->fin = NULL;
    lista->cantidad = 0;
    return lista; 
}

ListaDoble lde_crear(FuncionComparadora comp, FuncionCopiadora copy, FuncionDestructora destr, FuncionVisitante visit){
    ListaDoble lista = malloc(sizeof(struct Doble));
    assert(lista != NULL);
    lista->comp = comp;
    lista->copy = copy;
    lista->destroy = destr;
    lista->visit = visit;
    lista->inicio = NULL;
    lista->fin = NULL;
    lista->cantidad = 0;
    return lista; 
}

static void destruir_snodo(Snodo* lista, FuncionDestructora destroy){
    Snodo* temp = lista;
    while(temp != NULL){
        Snodo* actual = temp;
        temp = temp->sig;
        destroy(actual->dato);
        free(actual);
    }
}
void lse_destruir(ListaSimple lista){
    destruir_snodo(lista->inicio, lista->destroy);
    free(lista);
    return;
}

static void destruir_dnodo(Dnodo* lista, FuncionDestructora destroy){
    Dnodo* temp = lista;
    while(temp != NULL){
        Dnodo* actual = temp;
        temp = temp->sig;
        destroy(actual->dato);
        free(actual);
    }
}
void lde_destruir(ListaDoble lista){
    destruir_dnodo(lista->inicio, lista->destroy);
    free(lista);
    return;
}

static Snodo* lse_nodoAgregar(Snodo* lista, void* dato, FuncionCopiadora copy){
    Snodo* nuevo = malloc(sizeof(Snodo));
    assert(nuevo != NULL);
    nuevo->dato = copy(dato);
    nuevo->sig = lista;
    return nuevo;
}
void lse_agregarInicio(ListaSimple lista, void* dato){
    lista->inicio =  lse_nodoAgregar(lista->inicio, dato, lista->copy);
    if(lista->fin == NULL){
        lista->fin = lista->inicio;
    }
    lista->cantidad++;
}

static Dnodo* lde_nodoAgregar(Dnodo* lista, void* dato, FuncionCopiadora copy){
    Dnodo* nuevo = malloc(sizeof(Dnodo));
    assert(nuevo != NULL);
    nuevo->ant = NULL;
    nuevo->dato = copy(dato);
    nuevo->sig = lista;
    if(lista != NULL) lista->ant = nuevo;   
    return nuevo;
}
void lde_agregarInicio(ListaDoble lista, void* dato){
    lista->inicio = lde_nodoAgregar(lista->inicio, dato, lista->copy);
    if(lista->fin == NULL) lista->fin = lista->inicio;
    lista->cantidad++;
}

static Snodo* lse_eliminarDato(Snodo* lista, void* dato, FuncionDestructora destroy, FuncionComparadora comp, 
    Snodo** fin, int* cantidad){
    
    Snodo* temp = lista;
    Snodo* ant = NULL;
    while(temp != NULL && comp(temp->dato, dato) != 0){
        ant = temp;
        temp = temp->sig;
    } 
    if(temp == NULL) return lista;
    if(ant == NULL){
        lista = temp->sig;
    }
    else{
        ant->sig = temp->sig;
    }
    if(temp == *fin) *fin = ant;

    (*cantidad)--;

    destroy(temp->dato);
    free(temp);
    return lista;
}
void lse_eliminar(ListaSimple lista, void* dato){
    lista->inicio = lse_eliminarDato(lista->inicio, dato, lista->destroy, lista->comp, &lista->fin, &lista->cantidad);
}

static Dnodo* lde_eliminarDato(Dnodo* lista, void* dato, FuncionDestructora destroy, FuncionComparadora comp,
Dnodo** fin, int* cantidad){

    Dnodo* temp = lista;
    while(temp != NULL && comp(temp->dato, dato) != 0){
        temp = temp->sig;
    } 
    if(temp == NULL) return lista;
    if(temp->ant == NULL){
        lista = temp->sig;
        if(lista != NULL) lista->ant = NULL;
    }
    else{
        temp->ant->sig = temp->sig;
        if(temp->sig != NULL){
            temp->sig->ant = temp->ant;
        }
        else{
            *fin = temp->ant;
        }    
    }
    (*cantidad)--;

    destroy(temp->dato);
    free(temp);
    return lista;
}
void lde_eliminar(ListaDoble lista, void* dato){
    lista->inicio = lde_eliminarDato(lista->inicio, dato, lista->destroy, lista->comp, &lista->fin, &lista->cantidad);
}


void snodo_recorrer(Snodo* lista, FuncionVisitante visit){
    Snodo* temp = lista;
    while(temp != NULL){
        visit(temp->dato);
        temp = temp->sig;
    }
    return;
}
void lse_recorrer(ListaSimple lista){
    snodo_recorrer(lista->inicio, lista->visit);
}

void dnodo_recorrer(Dnodo* lista, FuncionVisitante visit){
    Dnodo* temp = lista;
    while(temp != NULL){
        visit(temp->dato);
        temp = temp->sig;
    } 
    return;
}
void lde_recorrer(ListaDoble lista){
    dnodo_recorrer(lista->inicio, lista->visit);
}

static Snodo* snodo_agregarfinal(Snodo* inicio, Snodo** final, void* dato, FuncionCopiadora copy){
    Snodo* nuevo = malloc(sizeof(Snodo));
    nuevo->dato = copy(dato);
    nuevo->sig = NULL;
    (*final)->sig = nuevo;
    *final = nuevo;
    return inicio;
}
void lse_agregarFinal(ListaSimple lista, void* dato){
    if(lista->inicio == NULL) {
        lse_agregarInicio(lista, dato);
        return;
    }
    lista->inicio = snodo_agregarfinal(lista->inicio, &lista->fin, dato,lista->copy);
    lista->cantidad++;
}

static Dnodo* dnodo_agregarfinal(Dnodo* inicio, Dnodo** final, void* dato, FuncionCopiadora copy){
    Dnodo* nuevo = malloc(sizeof(Dnodo));
    assert(nuevo != NULL);
    nuevo->dato = copy(dato);
    nuevo->sig = NULL;
    nuevo->ant = *final;
    (*final)->sig = nuevo;
    *final = nuevo;
    return inicio;
}

void lde_agregarFinal(ListaDoble lista, void* dato){
    if(lista->inicio == NULL) {
        lde_agregarInicio(lista, dato);
        return;
    }
    lista->inicio = dnodo_agregarfinal(lista->inicio, &lista->fin, dato, lista->copy);
    lista->cantidad++;
}

static Snodo* merge(Snodo* primera, Snodo* segunda, FuncionComparadora comp){
    if(primera == NULL) return segunda;
    if(segunda == NULL) return primera;

    Snodo* res = NULL;
    if(comp(primera->dato, segunda->dato) <= 0){
        res = primera;
        res->sig = merge(primera->sig, segunda, comp);
    }
    else if(comp(primera->dato, segunda->dato) > 0){
        res = segunda;
        res->sig = merge(primera, segunda->sig, comp);
    }

    return res;

}

static Snodo* mergesort(Snodo* cabeza, FuncionComparadora comp){
    if(cabeza == NULL || cabeza->sig == NULL) return cabeza;
    
    Snodo* tortuga = cabeza;
    Snodo* liebre = cabeza->sig;
    while(liebre != NULL && liebre->sig != NULL){
        tortuga = tortuga->sig;
        liebre = liebre->sig->sig;
    }
    Snodo* mitad = tortuga->sig;
    tortuga->sig = NULL;
    Snodo* primer = mergesort(cabeza, comp);
    Snodo* segunda = mergesort(mitad, comp);

    return merge(primer, segunda, comp); 
}

void SortMerge(ListaSimple lista){
    lista->inicio = mergesort(lista->inicio, lista->comp);
    Snodo* temp = lista->inicio;
    while(temp->sig != NULL) temp = temp->sig;
    lista->fin = temp;
}