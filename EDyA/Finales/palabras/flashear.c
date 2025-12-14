#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

typedef struct _Nodo{
    struct _Nodo* letras[26];
    int fin;
    int cant;
}Nodo;

typedef Nodo* Arbol;

Arbol crear_arbol(){
    Arbol tree = malloc(sizeof(Nodo));
    assert(tree != NULL);
    tree->cant = 0;
    tree->fin = 0;
    for(int i = 0; i<26; i++){
        tree->letras[i] = NULL;
    }
    return tree;
}


Arbol insertar_palabra(Arbol arbol,char* palabra){
    int len = strlen(palabra);
    Nodo* temp = arbol;
    temp->cant++;
    for(int i = 0; i < len; i++){
        int idx = tolower(palabra[i]) - 'a';
        if(temp->letras[idx] == NULL){
            temp->letras[idx] = crear_arbol();
        }
    
        temp = temp->letras[idx];
        temp->cant++;

    }
    temp->fin = 1;
    return arbol;
}



void imprimir_desde_nodo(Arbol nodo, char* impreso, int level){
    if(nodo->fin){
        impreso[level] = '\0';
        printf("Palabra: %s\n", impreso);
    }
    for(int i = 0; i<26; i++){
        if(nodo->letras[i] != NULL){
            char letra  = i + 'a';
            impreso[level] = letra;
            imprimir_desde_nodo(nodo->letras[i], impreso, level + 1);
        }
    }
}


void buscar_3_caracteres(Arbol arbol, char* chr){
    Nodo* temp = arbol;
    int len = 3;
    char buff[100];
    for(int i = 0; i < len; i++){
        buff[i] = chr[i];
        temp = temp->letras[tolower(chr[i])-'a'];
    }
    printf("Palabras que comienzan con %s: \n", chr);
    imprimir_desde_nodo(temp, buff, 3);
}

int ocurrencias(Arbol arbol, char c){
    return arbol->letras[c-'a']->cant;
}


// Tengo que entrar al arbol[i] que coincida con string[j] i va de 0 a 25, j va de 0 a strlen
// Mientras me voy metiendo a esos arbol[i] tengo que eliminar el dato y borrar la cantidad de ocurrencias
// cuando llego al ultimo cambio la bandera de fin de palabra a 0
int es_hoja(Arbol nodo){
    for(int i = 0; i<26;i++){
        if(nodo->letras[i] != NULL) return 0;
    }
    return 1;
}


Arbol eliminar(Arbol arbol, char* s){
    if(arbol == NULL) return NULL;
    arbol->cant--;
    if(*s == '\0'){
        if(arbol->fin){
            arbol->fin = 0;
        }
        if(es_hoja(arbol)){
            free(arbol);
            return NULL;
        }
        return arbol;
    }
    int idx = *s - 'a';
    arbol->letras[idx] = eliminar(arbol->letras[idx], s+1);

    if(arbol->fin == 0 && es_hoja(arbol)){
        free(arbol);
        return NULL;
    }
    return arbol;
}


typedef struct _GNodo{
    void* dato;
    struct _GNodo* sig;
}GNodo;

typedef GNodo* GList;

GList crear_glist(){
    return NULL;
}

typedef int(*FuncionComparadora)(void* dato1, void* dato2);


int longitud_glist(GList lista){
    GNodo* temp = lista;
    if(lista == NULL) return 0;
    int contador = 0;
    while(temp != NULL){
        temp = temp->sig;
        contador++;
    }
    return contador;
}

GList unir_listas(GList l1, GList l2, GList l3, FuncionComparadora comp){
    GList resultado = NULL;
    GList ultimo = NULL;

    while(l1 != NULL || l2 != NULL || l3 != NULL){
        GList* lower = NULL;

        if(l1 != NULL){
            lower = &l1;
        }
        if(l2 != NULL){
            if(lower == NULL){
                lower = &l2;
            }
            else{
                if(comp(l2->dato, (*lower)->dato) < 0){
                    lower = &l2;
                }
            }
        }

        if(l3 != NULL){
            if(lower == NULL){
                lower = &l3;
            }else{
                if(comp(l3->dato, (*lower)->dato)<0){
                    lower = &l3;
                }
            }
        }

        GNodo* nodo = malloc(sizeof(GNodo));
        nodo->dato = (*lower)->dato;
        nodo->sig = NULL;

        if(resultado == NULL){
            resultado = nodo;
            ultimo = nodo;
        }
        else{
            ultimo->sig = nodo;
            ultimo = nodo;
        }

        *lower = (*lower)->sig;

    }
    
    return resultado;
}

GList ordenar_lista(GList lista, FuncionComparadora comp){
    if(lista == NULL || lista->sig == NULL){
        return lista;
    }
    int len = longitud_glist(lista);
    GList l1 = lista;
    GList l2 = NULL;
    GList l3 = NULL;

    GList temp = lista;
    int k = 0;
    while(temp != NULL && k < (len/3)-1){
        temp = temp->sig;
        k++;
    }

    if(temp != NULL){
        l2 = temp->sig;
        temp->sig = NULL;
    }
    if(l2 != NULL){
        temp = l2;
        int restante = (len - len/3);
        k = 0;
        while(temp != NULL && k < (restante/2)-1){
            temp = temp->sig;
            k++;
        }
        if(temp != NULL){
            l3 = temp->sig;
            temp->sig = NULL;
        }
    }
    l1 = ordenar_lista(l1, comp);
    l2 = ordenar_lista(l2, comp);
    l3 = ordenar_lista(l3, comp);

    return unir_listas(l1, l2,l3, comp);

}

GList agregar(GList lista, void* dato){
    GNodo* nuevo = malloc(sizeof(GNodo));
    nuevo->dato = dato;
    nuevo->sig = lista;
    return nuevo;
}

int comparar(void* dato1, void* dato2){
    return *(int*)dato1 - *(int*)dato2;
}

int main(){
    Arbol trie = crear_arbol();
    insertar_palabra(trie, "hola");
    insertar_palabra(trie, "holanda");
    insertar_palabra(trie, "casa");
    insertar_palabra(trie, "holpala");
    insertar_palabra(trie, "gorlami");
    insertar_palabra(trie, "hoyo");


    char buffer[100]; // Buffer temporal para reconstruir palabras
    printf("Palabras metidas:\n");
    imprimir_desde_nodo(trie, buffer, 0);
    puts("");

    buscar_3_caracteres(trie, "hol");

    char c = 'h';
    int ocu = ocurrencias(trie, c);
    puts("");
    printf("La cantidad de ocurrencias con la letra %c es %d\n", c, ocu);

    trie = eliminar(trie, "hola");

    imprimir_desde_nodo(trie, buffer, 0);

    puts("");

    GList glist = crear_glist();
    
    // Array de 6 números
    int numeros[6] = {3, 6, 2, 9, -5, 1};
    
    // Insertamos todos (Indices 0 al 5)
    for(int i = 0; i < 6; i++){
        // Pasamos la dirección de memoria de cada número del array
        glist = agregar(glist, &numeros[i]);
    }

    printf("Lista original (invertida por agregar al inicio): \n");
    for(GList temp = glist; temp != NULL; temp = temp->sig){
        printf("%d ", *(int*)temp->dato);
    }
    printf("\n");

    // Ordenamos

    GList ordenada = ordenar_lista(glist, (FuncionComparadora)comparar);
    
    printf("Lista ordenada: \n");
    for(GList temp = ordenada; temp != NULL; temp = temp->sig){
        printf("%d ", *(int*)temp->dato);
    }
    printf("\n");



    return 0;

    return 0;
}