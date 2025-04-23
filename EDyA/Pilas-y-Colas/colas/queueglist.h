#ifndef ___QUEUEGLIST_H___
#define ___QUEUEGLIST_H___

typedef void (*FuncionDestructora)(void* dato);
typedef void (*FuncionVisitante)(void* dato);
typedef void *(*FuncionCopia)(void* dato);
typedef int (*FuncionComparadora)(void* dato1, void* dato2);


typedef struct _GNode {
  void *data;
  struct _GNode *next;
} GNode;

typedef GNode *GList;

GList glist_crear();

void glist_destruir(GList lista, FuncionDestructora destroy);

int glist_vacia(GList lista);

void  glist_recorrer(GList lista, FuncionVisitante f);

GList glist_insertar(GList lista, void* dato, FuncionCopia c, FuncionComparadora comp);

int glist_buscar(GList lista, void* dato, FuncionComparadora f);

GList glist_arr(void **arr, int len, FuncionCopia f, FuncionComparadora comp);

#endif