#include <stdlib.h>
#include <stdio.h>

typedef unsigned (*FuncionHash)(int);

typedef enum{
    LIBRE,
    OCUPADO,
    BORRADO,
}Estado;

typedef struct _Casilla{
    int dato;
    Estado estado;
}Casilla;

struct _TablaHash{
    Casilla* elementos;
    int numElems;
    int cantidad;
    FuncionHash hashear;
};

typedef struct _TablaHash* TablaHash;

TablaHash tabla_crear(int cantidad, FuncionHash hash){
    TablaHash tabla = malloc(sizeof(struct _TablaHash));
    tabla->elementos = malloc(sizeof(Casilla) * cantidad);
    tabla->cantidad = cantidad; tabla->numElems = 0;
    tabla->hashear = hash;
    for(int i = 0; i < cantidad; i++){
        tabla->elementos[i].dato = -1;
        tabla->elementos[i].estado = LIBRE;
    }
    return tabla;
}

void tabla_destruir(TablaHash tabla){
    free(tabla->elementos);
    free(tabla);
}

void rehashear(TablaHash tabla){
    int nuevaCap = tabla->cantidad * 2;
    Casilla* nuevosElems = malloc(sizeof(Casilla)* nuevaCap);
    for(int i = 0; i < nuevaCap; i++){
        nuevosElems[i].dato = -1;
        nuevosElems[i].estado = LIBRE;
    }

    for(int i = 0; i < tabla->cantidad; i++){
        if(tabla->elementos[i].estado == OCUPADO){
            unsigned nuevoIdx = tabla->hashear(tabla->elementos[i].dato) % nuevaCap;
            while(nuevosElems[nuevoIdx].estado == OCUPADO) nuevoIdx = (nuevoIdx + 1) % nuevaCap;
            printf("%d ", tabla->elementos[i].dato);
            nuevosElems[nuevoIdx].dato = tabla->elementos[i].dato;
            nuevosElems[nuevoIdx].estado = OCUPADO;
        }
    }
    free(tabla->elementos);
    tabla->elementos = nuevosElems;
    tabla->cantidad = nuevaCap;
}

float factor_balance(TablaHash tabla){
    return (float)tabla->numElems / tabla->cantidad;
}

void tablahash_insertar(TablaHash tabla, int dato){
    if(factor_balance(tabla) >= (float)0.7) rehashear(tabla);
    
    unsigned idx = tabla->hashear(dato) % tabla->cantidad;
    int i = idx; int borrado = -1;
    do{
        if(tabla->elementos[idx].estado == LIBRE){
            tabla->elementos[idx].dato = dato;
            tabla->elementos[idx].estado = OCUPADO;
            tabla->numElems++;
            return;
        }
        else if(tabla->elementos[idx].estado == BORRADO){
            if(borrado == -1) borrado = idx;
        }
        else if (tabla->elementos[idx].dato == dato) return;
        idx = (idx+1) % tabla->cantidad;
    }while(i != idx);
    if(borrado != -1){
        tabla->elementos[borrado].dato = dato;
        tabla->elementos[borrado].estado = OCUPADO;
        tabla->numElems++;
        return;
    }
}

int tablahash_buscar(TablaHash tabla, int dato){
    unsigned idx = tabla->hashear(dato) % tabla->cantidad; int i = idx;
    do{
        if(tabla->elementos[idx].estado == OCUPADO){
            if(tabla->elementos[idx].dato == dato) return dato;
        }
        else if(tabla->elementos[idx].estado == LIBRE) return -1;
        idx = (idx+1) % tabla->cantidad;
    }while(i != idx);
    return -1;
}

void tablahash_eliminar(TablaHash tabla, int dato){
    unsigned idx = tabla->hashear(dato) % tabla->cantidad;
    int i = idx;
    do{
        if(tabla->elementos[idx].estado == OCUPADO){
            if(tabla->elementos[idx].dato == dato){
                tabla->elementos[idx].dato = -1; tabla->numElems--;
                tabla->elementos[idx].estado = BORRADO;
            } 
            
        }
        else if(tabla->elementos[idx].estado == LIBRE) return;
        else if(tabla->elementos[idx].estado == BORRADO) idx = (idx+1) % tabla->cantidad;
        idx = (idx+1) % tabla->cantidad;
    }while(i != idx);
    return;
}

void imprimir_hash(TablaHash hash){
    for(int i = 0; i < hash->cantidad; i++){
        if(hash->elementos[i].estado == OCUPADO) printf("%d ", hash->elementos[i].dato);
    }
}



unsigned Hashear(int dato){ return dato; }

int main(){
    TablaHash tabla = tabla_crear(10, Hashear);
    int meter[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    
    printf("--- Inserción ---\n");
    for(int i = 0; i < 8; i++){
        tablahash_insertar(tabla, meter[i]);
        printf("Insertado %d | Factor: %0.2f\n", meter[i], factor_balance(tabla));
    }
    imprimir_hash(tabla);

    printf("\n--- Eliminación de 2 y 4 ---\n");
    tablahash_eliminar(tabla, 2);
    tablahash_eliminar(tabla, 4);
    imprimir_hash(tabla);

    printf("\n--- Inserción post-eliminación (Prueba de BORRADO) ---\n");
    tablahash_insertar(tabla, 12); // Debería ocupar hueco del 2
    imprimir_hash(tabla);

    printf("\n--- Destrucción ---\n");
    printf("Capacidad Final: %d, NumElems: %d \n", tabla->cantidad, tabla->numElems);
    tabla_destruir(tabla);
    
    return 0;
}