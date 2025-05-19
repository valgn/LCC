# Compilador
CC = gcc
# Opciones de compilación
CFLAGS = -Wall -g

# Archivos fuente y objeto
OBJ = bstree.o test.o

# Nombre del ejecutable
EXEC = test

# Regla por defecto
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

btree.o: bstree.c bstree.h
	$(CC) $(CFLAGS) -c btree.c

test.o: test.c bstree.h
	$(CC) $(CFLAGS) -c test.c

# Limpiar archivos compilados
clean:
	rm -f *.o $(EXEC)