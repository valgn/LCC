#ifndef ___TABLA_H___
#define ___TABLA_H___

typedef void *(*FuncionCopiadora)(void *dato);
typedef int (*FuncionComparadora)(void *dato1, void *dato2);
typedef void (*FuncionDestructora)(void *dato);
typedef int (*FuncionHash)(void *dato);

typedef enum
{
    VACIO,
    OCUPADO,
    BORRADO,
} EstadoCasilla;

typedef struct
{
    void *dato;
    EstadoCasilla estado;
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

typedef struct _TablaHash *TablaHash;

typedef struct
{
    char *nombre, *direccion, *dni;
    int edad;
} Persona;

#endif