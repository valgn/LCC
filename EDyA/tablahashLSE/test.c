#include "contacto.h"
#include "tablahash.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define CAPACIDAD_INICIAL 11
/** Capacidad inicial para la tabla hash */

/**
 * Caso de prueba: Tabla hash para contactos
 */
#define CAP_TEST 5 

int main() {
    printf("=== INICIO TEST DE REHASHING ===\n");

    // 1. Crear tabla diminuta
    TablaHash tabla = tablahash_crear(CAP_TEST, 
                                      (FuncionCopiadora)contacto_copia, 
                                      (FuncionComparadora)contacto_comparar, 
                                      (FuncionDestructora)contacto_destruir, 
                                      (FuncionHash)contacto_heashear);

    printf("Capacidad Inicial: %d\n", tablahash_capacidad(tabla));
    assert(tablahash_capacidad(tabla) == CAP_TEST);

    // Datos de prueba (6 contactos para desbordar una tabla de 5)
    // Nombres cortos para simplificar
    char* nombres[] = {"Ana", "Beto", "Carla", "Dani", "Eli", "Fran"};
    Contacto* contactos[6];

    // 2. Inserción con monitoreo
    for (int i = 0; i < 6; i++) {
        contactos[i] = contacto_crear(nombres[i], "111", 20 + i);
        
        printf("Insertando %s... ", nombres[i]);
        tablahash_insertar(tabla, contactos[i]);
        
        // Medimos factor de carga actual para ver qué pasa
        int cap_actual = tablahash_capacidad(tabla);
        int num_elems = tablahash_nelems(tabla);
        float carga = (float)num_elems / cap_actual;
        
        printf("[N: %d | Cap: %d | Carga: %.2f]\n", num_elems, cap_actual, carga);

        // CHEQUEO DE REHASH:
        // Si acabamos de insertar el 4to elemento en capacidad 5, la carga era 0.8.
        // Si tu lógica funciona, la capacidad debería haber saltado AHORA.
        if (i == 3 && cap_actual > CAP_TEST) { 
             printf("\t>>> ¡REHASH DETECTADO! La tabla creció <<<\n");
        }
    }

    // 3. Verificación Post-Rehash (La prueba de fuego)
    printf("\n=== VERIFICANDO INTEGRIDAD DE DATOS ===\n");
    printf("Capacidad Final: %d\n", tablahash_capacidad(tabla));
    
    // La capacidad debería ser mayor a la inicial (ej: 10 u 11)
    assert(tablahash_capacidad(tabla) > CAP_TEST); 

    int encontrados = 0;
    for (int i = 0; i < 6; i++) {
        // Buscamos usando el contacto original. 
        // Si el rehash falló moviendo punteros, esto devolverá NULL o crasheará.
        Contacto* buscado = tablahash_buscar(tabla, contactos[i]);
        
        if (buscado != NULL) {
            printf("OK: %s encontrado.\n", buscado->nombre);
            encontrados++;
        } else {
            printf("ERROR FATAL: %s se perdió en el rehash.\n", nombres[i]);
        }
    }

    assert(encontrados == 6);
    printf("\n>>> TEST PASADO: Todos los datos sobrevivieron a la mudanza.\n");

    // Limpieza
    tablahash_destruir(tabla);
    for(int i=0; i<6; i++) contacto_destruir(contactos[i]);

    return 0;
}