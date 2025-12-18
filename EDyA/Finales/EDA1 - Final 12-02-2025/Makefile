FLAGS=-Wall -Wextra -g -Werror

main: main.o btree.o
	$(CC) $^ $(FLAGS) -o $@

main.o: main.c btree.o
	$(CC) $< $(FLAGS) -c

btree.o: btree.c 
	$(CC) $< $(FLAGS) -c

clean:
	rm *.o
	rm main