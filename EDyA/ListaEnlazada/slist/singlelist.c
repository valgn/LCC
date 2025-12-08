#include <stdio.h>
#include <stdlib.h>


struct Snodo{
    int dato;
    struct Snodo* next;
};

struct Snodo* slist_crear() {
    return NULL;
}


struct Snodo* agregarlista(struct Snodo* head, int dato){
    struct Snodo* nuevonodo = malloc(sizeof(struct Snodo));
    nuevonodo->dato = dato;
    nuevonodo->next = head;
    return nuevonodo;
}

int slist_length(struct Snodo* head){
    struct Snodo* temp;
    temp = head;
    int i = 0;
    while(temp != NULL){
        i++;
        temp = temp->next;
        
    }

    return i;
}

void slist_concatenar(struct Snodo** head, struct Snodo** head2){
    struct Snodo* temp = (*head);

    for(;temp->next!=NULL;temp = temp->next);
    temp->next = *head2;
}

void slist_recorrer(struct Snodo* lista) {
    for (struct Snodo *nodo = lista; nodo != NULL; nodo = nodo->next){
        printf("%d\n",nodo->dato);
    }

}

struct Snodo* slist_insertar(struct Snodo* head, int dato, int pos){
    struct Snodo* temp = head;
    
    if (pos == 0) {
        struct Snodo* nuevonodo = malloc(sizeof(struct Snodo));
        nuevonodo->dato = dato;
        nuevonodo->next = head;
        return nuevonodo;
    }

    

    for(int i = 0;i < pos - 1 && temp != NULL;temp = temp->next, i++);
    
    if (temp == NULL)
    {
        perror("Para amigo mira la pos que me pones");
        return head;
    }
    
    struct Snodo* nuevonodo = malloc(sizeof(struct Snodo));
    nuevonodo->dato = dato;
    nuevonodo->next = temp->next;
    temp->next = nuevonodo;  
    
    return head;

}

struct Snodo* slist_eliminar(struct Snodo* head, int pos){

    struct Snodo* temp = head;
    if (pos == 0){
        temp = temp->next;
        return temp;
    }



    for(int i = 0;i < pos - 1 && temp->next != NULL;temp = temp->next, i++);

    temp->next = temp->next->next;
    return head;


}

int slist_contine(struct Snodo* head , int dato){
    struct Snodo* temp = head; 
    int esta= 0;
    
    for (;temp !=NULL;temp= temp -> next){
    if(temp->dato== dato){
        esta = 1;
    } 
    }
    return esta;
}

int slist_indice(struct Snodo* head, int dato){
    //struct Snodo* temp = head;
    int esta = -1;
    int pos;
    for(pos = 0;esta != 1 && head != NULL;head = head->next){
        if(head->dato != dato){
            pos++;
        }
        else{
            esta = 1;
        }
        
    }
    if(esta == 1){
        return pos;
    }
    else return -1;
}

struct Snodo* slist_intersecar(struct Snodo* head, struct Snodo* head2){
    struct Snodo* nuevonodo = malloc(sizeof(struct Snodo));

    for(;head != NULL;head = head->next){
        for(int pos = 0;head2 != NULL;head2 = head2->next){
            if(head->dato == head2->dato){
                nuevonodo = agregarlista(nuevonodo,head->dato);
                head2 = slist_eliminar(head2,pos);

            }
            pos++;
        }

        


    }
    return nuevonodo;
}

struct Snodo* slist_agregar_final(struct Snodo* lista, int dato) {
    struct Snodo* nuevoNodo = malloc(sizeof(struct Snodo));
    nuevoNodo->dato = dato;
    nuevoNodo->next = NULL;
  
    if (lista == NULL)
      return nuevoNodo;
  
    struct Snodo* nodo = lista;
    for (;nodo->next != NULL;nodo = nodo->next);
    /* ahora 'nodo' apunta al ultimo elemento en la lista */
  
    nodo->next = nuevoNodo;
    return lista;
  }


// void slist_partir(struct Snodo** lista, struct Snodo* primeramitad, struct Snodo*segundamitad){
//     struct Snodo* temp = lista;
//     int len = slist_length(lista);
//     int i = 0;
//     for(;i<(len/2)+1;i++, temp = temp = temp->next){
//         *primeramitad = slist_agregar_final(*primeramitad, temp->dato);


//     }
//     for(;i<len;i++, temp = temp->next){
//         *segundamitad = slist_agregar_final(*segundamitad,temp->dato);
//     }
// }

// struct Snodo* list_partir2(struct SNodo *lista){
//     struct SNodo* temp = lista;
//     int len = slist_length(lista);
//     for(int i = 0; i<(len-1)/2;i++, temp = temp->next);

//     struct SNodo* segunda_mitad = temp->next;
//     temp->next = NULL;
//     return segunda_mitad;

// }

int menorque(int a, int b){
    if (a < b){
        return 1;
    }
    else if (a > b)
    {
        return -1;
    }
    else return 0;
    
}

int mayorque(int a, int b){
    if (a>b){
        return 1;
    }
    else if (a < b){
        return -1;
    }
    else return 0;
}


struct Snodo* slist_ordenar(struct Snodo* head, int (*f)(int a, int b)){
    int tincho;
    struct Snodo* temp = head;
    struct Snodo* temp2;
    for(;temp!=NULL; temp = temp->next){
        //printf("chau");
        temp2 = temp->next;
        for(;temp2 !=NULL; temp2 = temp2->next){
            //printf("Hola");
            if((*f)(temp->dato, temp2->dato) < 0){
                tincho = temp2->dato;
                temp2->dato = temp->dato;
                temp->dato = tincho;
            }
            //else break;
        }
    }
    return head;
} 

struct Snodo* slist_reverso(struct Snodo* head){
    struct Snodo* prev = NULL;
    struct Snodo* curr = head;

    for (;curr != NULL;){
        
        struct Snodo* aux = curr->next;
        curr->next = prev;
        prev = curr;
        curr = aux;
    }
    return prev;
}


struct Snodo* slist_intercalar(struct Snodo* head, struct Snodo* head2){
    struct Snodo* temp = head;
    struct Snodo* temp2 = head2;
    struct Snodo* nuevonodo = NULL;
    
    for(;temp != NULL && temp2 != NULL;temp = temp->next, temp2= temp2->next){
        nuevonodo = agregarlista(nuevonodo, temp->dato);
        nuevonodo = agregarlista(nuevonodo, temp2->dato);
    }

    if (temp2 == NULL){
        for (;temp != NULL;temp= temp->next){
            nuevonodo = agregarlista(nuevonodo, temp->dato);
        }
    }
    
    if (temp == NULL){
        for (;temp2 != NULL;temp2 = temp2->next){
            nuevonodo = agregarlista(nuevonodo, temp2->dato);
        }
    }

    return nuevonodo;

}



int main(){
    // Inicialización de las listas
    struct Snodo* cabezon = slist_crear();
    cabezon = agregarlista(cabezon,4);
    cabezon = agregarlista(cabezon,3);
    cabezon = agregarlista(cabezon,2);
    cabezon = agregarlista(cabezon,1);

    struct Snodo* cabezon2 = slist_crear();
    cabezon2 = agregarlista(cabezon2,6);
    cabezon2 = agregarlista(cabezon2,5);

    
    // Concatenar las listas
    //slist_concatenar(&cabezon, &cabezon2);
    
    //printf("%d\n",slist_length(cabezon));
    //cabezon = slist_eliminar(cabezon,5);
    //printf("%d\n",slist_indice(cabezon,7));
    struct Snodo* nuevonodo = malloc(sizeof(struct Snodo));
    
    //slist_recorrer(cabezon);

    nuevonodo = slist_intercalar(cabezon, cabezon2);


    
    slist_recorrer(nuevonodo);





    return 0;
}