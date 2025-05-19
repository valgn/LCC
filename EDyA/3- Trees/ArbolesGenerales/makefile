# Compilador
CC = gcc
# Opciones de compilación
CFLAGS = -Wall -g

# Archivos fuente y objeto
OBJ = gbtree.o test.o contacto.o

# Nombre del ejecutable
EXEC = test

# Regla por defecto
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

gbtree.o: gbtree.c gbtree.h
	$(CC) $(CFLAGS) -c gbtree.c

contacto.o: contacto.c contacto.h
	$(CC) $(CFLAGS) -c contacto.c


test.o: test.c gbtree.h contacto.h
	$(CC) $(CFLAGS) -c test.c

# Limpiar archivos compilados
clean:
	rm -f *.o $(EXEC)