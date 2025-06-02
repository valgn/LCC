# Compilador
CC = gcc
# Opciones de compilación
CFLAGS = -Wall -g

# Archivos fuente y objeto
OBJ = avl.o test.o

# Nombre del ejecutable
EXEC = test

# Regla por defecto
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

avl.o: avl.c avl.h
	$(CC) $(CFLAGS) -c avl.c

test.o: test.c avl.h
	$(CC) $(CFLAGS) -c test.c

# Limpiar archivos compilados
clean:
	rm -f *.o $(EXEC)