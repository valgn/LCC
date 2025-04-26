FLAGS = -Wall -Wextra -Werror -std=c99

programa : main.o bullrich.o
	$(CC) -o $@ $^ $(FLAGS)

main.o: main.c extension.h
	$(CC) -c $@ $^ $(FLAGS)

bullrich.o: bullrich.c extension.h
	$(CC) -c $@ $^ $(FLAGS)

clean:
	rm *.o
	rm programa

.PHONY = clean