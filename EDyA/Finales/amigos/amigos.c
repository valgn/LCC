#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct _TNodo{
    char* nombre;
    struct _TNodo* sig;
}Amigo;

typedef struct _Persona{
    char* nombre; 
    Amigo* amigos; 
    struct _Persona* sig;
}Persona;

typedef struct{
    Persona* head;
}CasillaHash;



typedef unsigned (*FuncionHash)(char* dato);

struct _TablaHash{
    CasillaHash* elems;
    unsigned capacidad;
    FuncionHash hash;
};

typedef struct _TablaHash* S;

S crear_tabla(unsigned capacidad, FuncionHash hash){
    S tabla = malloc(sizeof(struct _TablaHash));
    tabla->elems = malloc( sizeof(CasillaHash) * capacidad );
    tabla->capacidad = capacidad;
    tabla->hash = hash;
    for(int i = 0; i<capacidad;i++){
        tabla->elems[i].head = NULL;
    }
    return tabla;
}

// La tabla hash tiene como llave el nombre de la persona y los datos de la key de la persona son los amigos con los que se 
//relaciona

struct _PNodo{
    char* nombre;
    struct _PNodo* sig;
};

typedef struct _PNodo* P;

Persona* buscar_persona(Persona* nodo, char* persona){
    Persona* temp = nodo;
    while(temp != NULL && strcmp(temp->nombre, persona) != 0){
        temp = temp->sig;
    }
    return temp;
}

void insertar_amigo(S tabla, char* nombre ,char* amigo){
    unsigned idx = tabla->hash(nombre) % tabla->capacidad;
    Persona* esta = buscar_persona(tabla->elems[idx].head, nombre);
    if(esta){
        Amigo* amg = malloc(sizeof(Amigo));
        amg->nombre = amigo;
        amg->sig = esta->amigos;
        esta->amigos = amg;
    }
    else{
        Persona* pers = malloc(sizeof(Persona));
        pers->nombre = nombre;
        pers->amigos = malloc(sizeof(Amigo));
        pers->amigos->nombre = amigo;
        pers->amigos->sig = NULL;
        pers->sig = tabla->elems[idx].head;
        tabla->elems[idx].head = pers;
    }
}

void crear_relacion(S tabla, char* p1, char* p2){
    insertar_amigo(tabla, p1, p2);
    insertar_amigo(tabla, p2, p1);
}
    
int son_amigos(S tabla, char* p1, char* p2){
    unsigned idx = tabla->hash(p1) % tabla->capacidad;
    Persona* encontrada = buscar_persona(tabla->elems[idx].head, p1);
    if(encontrada == NULL) return 0;
    Amigo* amigo = encontrada->amigos;
    while(amigo != NULL){
        if(strcmp(amigo->nombre, p2) == 0) return 1;
        amigo = amigo->sig;
    }
    return 0;
}

int es_amigo_de_todos(S tabla, P grupo_actual, char* candidato){
    unsigned idx = tabla->hash(candidato) % tabla->capacidad;
    P miembros = grupo_actual;

    while(miembros != NULL){
        if(son_amigos(tabla, candidato, miembros->nombre) == 0) return 0;
        miembros = miembros->sig;
    }
    return 1;
}

int longitud_lista(P lista){
    P temp = lista;
    int contador = 0;
    while(temp != NULL){
        temp = temp->sig;
        contador++;
    }
    return contador;
}

void destruir_lista(P lista){
    P temp = lista;
    while(temp != NULL){
        P temp2 = temp->sig;
        free(temp);
        temp = temp2;
    }
}

P agregar_inicio(P lista, char* dato){
    P nodo = malloc(sizeof(struct _PNodo));
    nodo->nombre = dato;
    nodo->sig = lista;
    return nodo;
}

P copiar_lista(P lista){
    if(lista == NULL) return NULL;
    P nuevo = malloc(sizeof(struct _PNodo));
    nuevo->nombre = lista->nombre; // El string no hace falta copiarlo si es literal
    nuevo->sig = copiar_lista(lista->sig);
    return nuevo;
}

void buscar_clique(S tabla, Amigo* candidatos, P grupo, P* mejorgrupo){
    // CASO BASE: No hay más candidatos
    if(candidatos == NULL){
        if(longitud_lista(grupo) > longitud_lista(*mejorgrupo)){
            destruir_lista(*mejorgrupo);
            *mejorgrupo = copiar_lista(grupo); // <--- COPIA SEGURA
        }
        return;
    }

    char* persona = candidatos->nombre;

    // --- RAMA 1: INCLUIR AL CANDIDATO ---
    if(es_amigo_de_todos(tabla, grupo, persona)){
        // 1. Agregamos (Malloc)
        P nodo_nuevo = malloc(sizeof(struct _PNodo));
        nodo_nuevo->nombre = persona;
        nodo_nuevo->sig = grupo;

        // 2. Recursión con el nuevo grupo
        buscar_clique(tabla, candidatos->sig, nodo_nuevo, mejorgrupo);

        // 3. Backtracking: LIBERAMOS el nodo antes de seguir (importante)
        free(nodo_nuevo); 
    }

    // --- RAMA 2: EXCLUIR AL CANDIDATO ---
    buscar_clique(tabla, candidatos->sig, grupo, mejorgrupo);
}

P omega(S relaciones){
    P mejorgrupo = NULL; // Empieza vacío

    for(int i = 0; i< relaciones->capacidad; i++){
        // Recorremos cada "Persona" en el bucket (por si hubo colisiones)
        Persona* p = relaciones->elems[i].head;
        while(p != NULL){
            
            // Creamos el grupo inicial CON EL ANFITRIÓN
            P grupo_inicial = malloc(sizeof(struct _PNodo));
            grupo_inicial->nombre = p->nombre;
            grupo_inicial->sig = NULL;

            // Buscamos la clique entre sus amigos
            buscar_clique(relaciones, p->amigos, grupo_inicial, &mejorgrupo);

            // Liberamos el grupo inicial temporal
            free(grupo_inicial);
            
            p = p->sig;
        }
    }
    return mejorgrupo;
}

unsigned KRHash(char *s) {
  unsigned hashval;
  for (hashval = 0; *s != '\0'; ++s) {
    hashval = *s + 31 * hashval;
  }
  return hashval;
}


int main(){
    S relaciones = crear_tabla(5, (FuncionHash)KRHash);
    crear_relacion(relaciones,"Juan", "Maria");
    crear_relacion(relaciones,"Maria", "Jorge");
    crear_relacion(relaciones,"Jorge", "Juan");
    crear_relacion(relaciones,"Jorge", "Jesus");
    P w = omega(relaciones);
    for(P temp = w; temp != NULL; temp = temp->sig){
        printf("%s \n", temp->nombre);
    }

    return 0;
}