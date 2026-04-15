#ifndef ___LISTAS_H___
#define ___LISTAS_H___

typedef void (*FuncionVisitante)(void*);
typedef void* (*FuncionCopiadora)(void*);
typedef int (*FuncionComparadora)(void*, void*);
typedef void (*FuncionDestructora)(void*);

typedef struct _SNodo{
    void* dato;
    struct _SNodo* sig;
}Snodo;

struct Simple{
    Snodo* inicio;
    Snodo* fin;
    int cantidad;
    FuncionVisitante visit;
    FuncionCopiadora copy;
    FuncionComparadora comp;
    FuncionDestructora destroy;
};
typedef struct Simple* ListaSimple;

typedef struct _DNodo{
    void* dato;
    struct _DNodo* sig, *ant;
}Dnodo;

struct Doble{
    Dnodo* inicio;
    Dnodo* fin;
    int cantidad;
    FuncionVisitante visit;
    FuncionCopiadora copy;
    FuncionComparadora comp;
    FuncionDestructora destroy;  
};
typedef struct Doble* ListaDoble;

ListaSimple lse_crear(FuncionComparadora comp, FuncionCopiadora copy, FuncionDestructora destr, FuncionVisitante visit);

ListaDoble lde_crear(FuncionComparadora comp, FuncionCopiadora copy, FuncionDestructora destr, FuncionVisitante visit);

void lse_destruir(ListaSimple lista);

void lde_destruir(ListaDoble lista);

void lse_agregarInicio(ListaSimple lista, void* dato);

void lde_agregarInicio(ListaDoble lista, void* dato);

void lse_eliminar(ListaSimple lista, void* dato);

void lde_eliminar(ListaDoble lista, void* dato);

void lse_recorrer(ListaSimple lista);

void lde_recorrer(ListaDoble lista);

void lse_agregarFinal(ListaSimple lista, void* dato);

void lde_agregarFinal(ListaDoble lista, void* dato);

void SortMerge(ListaSimple lista);

#endif