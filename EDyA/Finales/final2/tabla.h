#ifndef ___TABLA_H___
#define ___TABLA_H___

typedef void*(*FuncionCopiadora)(void* dato);
typedef int(*FuncionComparadora)(void* dato1, void* dato2);
typedef void(*FuncionDestructora)(void* datp);
typedef int(*FuncionHash)(void* dato);

typedef struct
{
    void *dato;
} CasillaHash;
struct _TablaHash
{
    CasillaHash *elems;
    unsigned numElems;
    unsigned capacidad;
    FuncionCopiadora copia;
    FuncionComparadora comp;
    FuncionDestructora destr;
    FuncionHash hash;
};

typedef struct _TablaHash* TablaHash;


#endif