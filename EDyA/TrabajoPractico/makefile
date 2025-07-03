# Compilador
CC = gcc
# Opciones de compilación
CFLAGS = -Wall -g

# Archivos fuente y objeto
OBJ = listas.o funciones.o parser.o main.o

# Nombre del ejecutable
EXEC = test

# Regla por defecto
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

listas.o: listas.c listas.h
	$(CC) $(CFLAGS) -c listas.c

funciones.o: funciones.c funciones.h
	$(CC) $(CFLAGS) -c funciones.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

main.o: main.c funciones.h parser.h
	$(CC) $(CFLAGS) -c main.c


# Limpiar archivos compilados
clean:
	rm -f *.o $(EXEC)