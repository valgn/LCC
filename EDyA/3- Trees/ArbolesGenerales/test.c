#include <stdio.h>
#include <stdlib.h>
#include "gbtree.h"
#include "contacto.h"



int main() {


  Contacto *contactos[6];
  contactos[0] = contacto_crear("Pepe Argento", "3412695452", 61);
  contactos[1] = contacto_crear("Moni Argento", "3412684759", 60);
  contactos[2] = contacto_crear("Coqui Argento", "3415694286", 32);
  contactos[3] = contacto_crear("Paola Argento", "3416259862", 29);
  contactos[4] = contacto_crear("Maria Elena Fuseneco", "3416874594", 59);
  contactos[5] = contacto_crear("Dardo Fuseneco", "3416894526", 64);

  
  BTree ll = btree_unir(contactos[3], btree_crear(), btree_crear(),(FuncionCopia)contacto_copia);
  BTree lr = btree_unir(contactos[4], btree_crear(), btree_crear(),(FuncionCopia)contacto_copia);
  BTree l = btree_unir(contactos[2], ll, lr, (FuncionCopia)contacto_copia);


  BTree rr = btree_unir(contactos[5], btree_crear(), btree_crear(),(FuncionCopia)contacto_copia);
  BTree r = btree_unir(contactos[1], btree_crear(), rr,(FuncionCopia)contacto_copia);
  BTree raiz = btree_unir(contactos[0], l, r, (FuncionCopia)contacto_copia);

  puts("");
  
  btree_recorrer(raiz, BTREE_RECORRIDO_IN, (FuncionVisitante)contacto_imprimir);
  
  btree_destruir(raiz, (FuncionDestructora)contacto_destruir);
  
  for(int i = 0; i < 6; i++){
    contacto_destruir(contactos[i]);
  }
  return 0;
}
