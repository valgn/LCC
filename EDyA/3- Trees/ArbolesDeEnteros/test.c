#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

static void imprimir_entero(int data) {
  printf("%d ", data);
}

void funcionloca(int dato, void* tincho){
  printf("%d", dato);

}



int main() {
  BTree lrl = btree_unir(8, btree_crear(), btree_crear());
  BTree lrr = btree_unir(9, btree_crear(), btree_crear());

  
  BTree ll = btree_unir(1, btree_crear(), btree_crear());
  BTree lr = btree_unir(7, lrl, lrr);
  BTree l = btree_unir(2, ll, lr);


  BTree rr = btree_unir(5, btree_crear(), btree_crear());
  BTree rl = btree_unir(6, btree_crear(), btree_crear());
  BTree r = btree_unir(3, rl, rr);
  BTree raiz = btree_unir(4, l, r);

  btree_recorrer(raiz, BTREE_RECORRIDO_PRE, imprimir_entero);
  puts("");
  BTree arbol2 = mirror(raiz);
  btree_recorrer(arbol2, BTREE_RECORRIDO_IN, (FuncionVisitante)imprimir_entero);
  printf("\n");
  btree_preorder(raiz, (FuncionVisitante)imprimir_entero);
  puts("");
  printf("\n");
  //imprimir_entero(btree_nnodos(raiz));
  //imprimir_entero(btree_buscar(raiz, 10));
  
  //BTree copia = btree_copiar(raiz);
  //btree_recorrer(copia, BTREE_RECORRIDO_POST, imprimir_entero);

  //imprimir_entero(btree_sumar(raiz));
  //int cantNodos = 0;
  //printf("%d", iesimo(raiz, 2, &cantNodos));
  btree_recorrer_bfs(raiz, imprimir_entero);
  puts("");
  bfs(raiz, imprimir_entero);
  puts("");
  
  
  btree_destruir(raiz);
  btree_destruir(arbol2);
  return 0;
}
