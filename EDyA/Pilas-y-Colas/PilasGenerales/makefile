# Makefile para P2E6
# ejecutar desde la terminal escribiendo "make" 
# genera automáticamente el ejecutable del programa

FLAGS = -Wall -Wextra -Werror -std=c99

programa: main.o gpilas.o glist.o contacto.o
	$(CC) -o $@ $^ $(FLAGS)

main.o: main.c gpilas.h glist.h contacto.h
	$(CC) -c $< $(FLAGS)

gpilas.o: gpilas.c gpilas.h glist.h
	$(CC) -c $< $(FLAGS)

glist.o: glist.c glist.h
	$(CC) -c $< $(FLAGS)

clean:
	rm *.o
	rm programa

.PHONY = clean