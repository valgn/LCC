# Makefile para P2E6
# ejecutar desde la terminal escribiendo "make" 
# genera automáticamente el ejecutable del programa

FLAGS = -Wall -Wextra -Werror -std=c99

programa: main.o queueglist.o contacto.o queue.o
	$(CC) -o $@ $^ $(FLAGS)

main.o: main.c queueglist.h contacto.h queue.h
	$(CC) -c $< $(FLAGS)

queueglist.o: queueglist.c queueglist.h
	$(CC) -c $< $(FLAGS)

contacto.o: contacto.c contacto.h
	$(CC) -c $< $(FLAGS)

queue.o: queue.c queue.h
	$(CC) -c $< $(FLAGS)


clean:
	rm *.o
	rm programa

.PHONY = clean