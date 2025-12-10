#include <stdio.h>
#include <stdlib.h>
#include "tabla.h"
#include <string.h>
#include <assert.h>


// -----------------------------------

// TABLA HASH
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
    tabla->elems[idx].dato= NULL;
    tabla->elems[idx].estado = VACIO;
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
    if (tabla->elems[idx].estado == OCUPADO){
        tabla->destr(tabla->elems[idx].dato);

    }
  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}



float factor_balance(TablaHash tabla){
    return (float)tabla->numElems / tabla->capacidad;
}

void rehashear(TablaHash tabla){
    unsigned nuevacant = tabla->capacidad * 2;
    CasillaHash* nuevos = malloc(sizeof(CasillaHash) * nuevacant);
    
    for(int i = 0; i< nuevacant; i++){
        nuevos[i].estado = VACIO;
        nuevos[i].dato = NULL;
    }


    for(int i = 0; i < tabla->capacidad; i++){
        if(tabla->elems[i].estado == OCUPADO){
            void* dato = tabla->elems[i].dato;
            unsigned newidx = tabla->hash(dato) % nuevacant;
            while(nuevos[newidx].estado == OCUPADO){
                newidx = (newidx+1)%nuevacant;
            }
            nuevos[newidx].dato = dato;
            nuevos[newidx].estado = OCUPADO;
        }
    }
    free(tabla->elems);
    tabla->capacidad = nuevacant;
    tabla->elems = nuevos;
}

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 * IMPORTANTE: La implementacion no maneja colisiones.
 */

 void tablahash_insertar(TablaHash tabla, void *dato) {
    if(factor_balance(tabla) > 0.75){
        rehashear(tabla);
    }

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
    unsigned idx = tabla->hash(dato) % tabla->capacidad;
    int i = idx, hueco = -1;
    int lugar = 0;
    do{
        if(tabla->elems[idx].estado == OCUPADO){
            if(tabla->comp(tabla->elems[idx].dato, dato) == 0){
            // Sobreescribir
            tabla->destr(tabla->elems[idx].dato);
            tabla->elems[idx].dato = tabla->copia(dato);
            return;
            }
        }
        else if(tabla->elems[idx].estado == VACIO){
            if(hueco == -1) hueco = idx;     
            lugar = 1;
        }
        else if(tabla->elems[idx].estado == BORRADO){
            if (hueco == -1) hueco = idx;
        }
        idx = (idx+1)%tabla->capacidad;     
    }
    while(i != idx && !lugar);

    if(hueco != -1){
        tabla->elems[hueco].dato = tabla->copia(dato);
        tabla->elems[hueco].estado = OCUPADO;
        tabla->numElems++;
    }

}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
    unsigned idx = tabla->hash(dato) % tabla->capacidad;
    int i = idx;
    do{
        if(tabla->elems[idx].estado == VACIO){
            return NULL;
        }
        if(tabla->elems[idx].estado == OCUPADO){
            if(tabla->comp(tabla->elems[idx].dato, dato) == 0){
                return tabla->elems[idx].dato;
            }
        }
        idx = (idx+1)%tabla->capacidad;
    }
    while(i != idx);

    return NULL;
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
    unsigned idx = tabla->hash(dato) % tabla->capacidad;
    int i = idx;
    do{
        if(tabla->elems[idx].estado == VACIO) return;
        if(tabla->elems[idx].estado == OCUPADO){
            if(tabla->comp(tabla->elems[idx].dato,dato) == 0){
                tabla->destr(tabla->elems[idx].dato);
                tabla->elems[idx].estado = BORRADO;
                tabla->numElems--;
            }
        }
        idx = (idx+1)%tabla->capacidad;
    }while(i != idx);
}

Persona* crear_persona(char* nombre, char* dni, char* direccion, int edad){
    Persona* persona = malloc(sizeof(Persona));
    persona->nombre = malloc(sizeof(char)*strlen(nombre)+1);
    strcpy(persona->nombre, nombre);
    persona->dni = malloc(sizeof(char)*strlen(dni)+1);
    strcpy(persona->dni, dni);
    persona->direccion = malloc(sizeof(char)*strlen(direccion)+1);
    strcpy(persona->direccion, direccion);
    persona->edad = edad;
    return persona;
}

Persona* copiar_persona(Persona* p){
    Persona* copia = crear_persona(p->nombre, p->dni, p->direccion, p->edad);
    return copia;
}

Persona* destruir_persona(Persona* p){
    free(p->direccion);
    free(p->dni);
    free(p->nombre);
    free(p);
}

int comparar_persona(Persona* p, Persona* q){
    return strcmp(p->dni, q->dni); // Si tienen el mismo dni son la misma persona
}

int KRHash(char* s){
    unsigned hashval;
    for(hashval = 0; *s != '\0'; ++s){
        hashval = *s + 32 * hashval;
    }
    return hashval;
}

int persona_hashear(Persona* p){
    return KRHash(p->dni);
}

void persona_imprimir(Persona* p){
    if(p == NULL){
        printf("No encontrado\n");
        return;
    }
    else{
        printf("{%s, %s, %s, %d}\n", p->nombre, p->dni, p->direccion, p->edad);
    }
}

int main(){
    Persona* personas[6];
    personas[0] = crear_persona("Pepe Argento", "3412695452","Avenida Rosario" ,61);
    personas[1] = crear_persona("Moni Argento", "3412684759", "Avenida siempreeviva",60);
    personas[2] = crear_persona("Coqui Argento", "3415694286", "Avenida Gorlomi" ,32);
    personas[3] = crear_persona("Paola Argento", "3416259862", "Avenida Decoco" ,29);
    personas[4] = crear_persona("Maria Elena Fuseneco", "3416874594", "Avenida Hans" ,59);
    personas[5] = crear_persona("Dardo Fuseneco", "3416894526", "Avenida 88" ,64);

    int capacidad = 6;
    TablaHash tabla = tablahash_crear(capacidad, (FuncionCopiadora)copiar_persona, (FuncionComparadora)comparar_persona, 
    (FuncionDestructora)destruir_persona, (FuncionHash)persona_hashear);

    for(int i = 0; i < 6; i++){
        tablahash_insertar(tabla, personas[i]);
    }

    Persona* encontrado;
    encontrado = tablahash_buscar(tabla, personas[1]);
    persona_imprimir(encontrado);
    
    tablahash_eliminar(tabla, personas[2]);
    Persona* encontrado2;
    encontrado2 = tablahash_buscar(tabla, personas[2]);
    persona_imprimir(encontrado2);

    for(int i = 0; i < 6; i++){
        destruir_persona(personas[i]);
    }

    tablahash_destruir(tabla);


    

    return 0;

}

