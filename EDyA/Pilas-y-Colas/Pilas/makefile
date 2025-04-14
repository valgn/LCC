# Makefile para P2E6
# ejecutar desde la terminal escribiendo "make" 
# genera automáticamente el ejecutable del programa

FLAGS = -Wall -Wextra -Werror -std=c99

programa: main.o pilas.o arregloenteros.o
	$(CC) -o $@ $^ $(FLAGS)

main.o: main.c pilas.h arregloenteros.h
	$(CC) -c $< $(FLAGS)

pilas.o: pilas.c pilas.h
	$(CC) -c $< $(FLAGS)

arregloenteros.o: arregloenteros.c arregloenteros.h
	$(CC) -c $< $(FLAGS)

clean:
	rm *.o
	rm programa

.PHONY = clean