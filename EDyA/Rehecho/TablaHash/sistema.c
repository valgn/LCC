#include <stdio.h>
#include <stdlib.h>

#define CANT_MATERIAS 5

typedef struct _NodoMateria{
    int id_materia;
    struct _NodoMateria* sig;
}NodoMateria;

typedef struct _NodoAlumno{
    int id;
    NodoMateria* materias;
    struct _NodoAlumno* sig;
}NodoAlumno;

struct _Sistema{
    NodoAlumno** alumnos;
    int capacidad;
    int contador_materias[CANT_MATERIAS];
};

typedef struct _Sistema* Sistema;

Sistema crear_sistema(int capacidad){
    Sistema sys = malloc(sizeof(struct _Sistema));
    sys->alumnos = malloc(sizeof(NodoAlumno*) * capacidad);
    sys->capacidad = capacidad;
    for(int i = 0; i < CANT_MATERIAS; i++) sys->contador_materias[i] = 0;
    for(int i = 0; i < sys->capacidad; i++){
        sys->alumnos[i] = NULL;
    }
    return sys;
}

NodoAlumno* buscar_o_crear(Sistema s, int padron){
    int indice = padron % s->capacidad;
    NodoAlumno* actual = s->alumnos[indice];
    while(actual != NULL){
        if(actual->id == padron) return actual;
        else actual = actual->sig;
    }
    NodoAlumno* nuevoAlumno = malloc(sizeof(NodoAlumno));
    nuevoAlumno->id = padron; nuevoAlumno->materias = NULL; nuevoAlumno->sig = s->alumnos[indice];
    s->alumnos[indice] = nuevoAlumno;

    return nuevoAlumno;
}

void inscribir(Sistema s, int id, int id_materia){
    if (id_materia < 0 || id_materia >= CANT_MATERIAS) return;
    int idx = id % s->capacidad;
    NodoAlumno* alumno = buscar_o_crear(s, id);
    
    NodoMateria* temp = alumno->materias;
    while(temp != NULL){
        if(temp->id_materia == id_materia) return;
        else temp = temp->sig;
    }
    NodoMateria* nuevaMateria = malloc(sizeof(NodoMateria));
    nuevaMateria->id_materia = id_materia;
    nuevaMateria->sig = alumno->materias;
    alumno->materias = nuevaMateria;

    s->contador_materias[id_materia]++;
}

void mostrar_estadisticas(Sistema s) {
    printf("--- ESTADISTICAS ---\n");
    for(int i=0; i<CANT_MATERIAS; i++) {
        printf("Materia %d: %d alumnos\n", i, s->contador_materias[i]);
    }
}


void studentDestroy(NodoAlumno* alumnos){
    NodoAlumno* temp = alumnos;
    while(temp != NULL){
        NodoAlumno* borrar = temp;
        temp = temp->sig;
        NodoMateria* materia = borrar->materias;
        while(materia != NULL){
            NodoMateria* borramateria = materia;
            materia = materia->sig;
            free(borramateria);
        }
        free(borrar);
    }
}

void systemDestroy(Sistema s){
    for(int i = 0; i < s->capacidad; i++){
        if(s->alumnos[i] != NULL)
        studentDestroy(s->alumnos[i]);
    }
    free(s->alumnos);
    free(s);
}

int main() {
    Sistema sys = crear_sistema(10);
    
    // Inscribimos alumnos
    inscribir(sys, 100, 1); // Padron 100 a Materia 1
    inscribir(sys, 200, 1); // Padron 200 a Materia 1
    inscribir(sys, 100, 2); // Padron 100 a Materia 2
    
    // Padron 100 ya existe, solo le agrega materia.
    // Padron 200 es nuevo en la misma casilla hash (colisión 100%10 == 200%10).
    
    mostrar_estadisticas(sys);
    
    // Acá faltaría una función sistema_destruir() bien recursiva
    systemDestroy(sys);
    return 0;
}