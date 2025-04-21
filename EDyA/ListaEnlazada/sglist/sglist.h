#ifndef __SGLIST_H__
#define __SGLIST_H__

typedef void (*FuncionDestructora)(void* dato);
typedef void (*FuncionVisitante)(void* dato);
typedef void *(*FuncionCopia)(void* dato);
typedef int (*FuncionComparadora)(void* dato1, void* dato2);


typedef struct _GNode {
  void *data;
  struct _GNode *next;

} GNode;

typedef GNode *GList;

typedef GList SGList;

SGList sglist_crear();

void sglist_destruir(SGList lista, FuncionDestructora destroy);

int sglist_vacia(SGList lista);

void  sglist_recorrer(GList lista, FuncionVisitante f);

SGList sglist_insertar(SGList lista, void* dato, FuncionCopia c, FuncionComparadora comp);

int sglist_buscar(GList lista, void* dato, FuncionComparadora f);

#endif 