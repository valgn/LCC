#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/*
 * Para probar, usar netcat. Ej:
 *
 *      $ nc localhost 4040
 *      NUEVO
 *      0
 *      NUEVO
 *      1
 *      CHAU
 */

void quit(char *s)
{
	perror(s);
	abort();
}


int fd_readline(int fd, char *buf)
{
    int rc;
	int i = 0;
    
	/*
    * Leemos de a un caracter (no muy eficiente...) hasta
    * completar una línea.
    */
   while ((rc = read(fd, buf + i, 1)) > 0) {
       if (buf[i] == '\n')
       break;
       i++;
	}
    
	if (rc < 0)
    return rc;
    
	buf[i] = 0;
	return i;
}

void* handle_conn(void* csock)
{
    int U = 0;
    char buf[200];
	int rc;
    
	while (1) {
		/* Atendemos pedidos, uno por linea */
		rc = fd_readline(*(int*)csock, buf);
		if (rc < 0)
			quit("read... raro");

		if (rc == 0) {
			/* linea vacia, se cerró la conexión */
			close(*(int*)csock);
			return NULL;
		}

		if (!strcmp(buf, "NUEVO")) {
			char reply[20];
			sprintf(reply, "%d\n", U);
			U++;
			write(*(int*)csock, reply, strlen(reply));
		} else if (!strcmp(buf, "CHAU")) {
			close(*(int*)csock);
			return NULL;
		}
	}
}

void wait_for_clients2(int  lsock)
{
	int csock;

	/* Esperamos una conexión, no nos interesa de donde viene */
	csock = accept(lsock, NULL, NULL);
	if (csock < 0)
		quit("accept");

    pthread_t hilo;

    pthread_create(&hilo, NULL, handle_conn, &csock);
    //pthread_join(hilo,NULL);
	/* Atendemos al cliente */


	/* Volvemos a esperar conexiones */
	wait_for_clients2(lsock);
}
/* Crea un socket de escucha en puerto 4040 TCP */
int mk_lsock()
{
	struct sockaddr_in sa;
	int lsock;
	int rc;
	int yes = 1;

	/* Crear socket */
	lsock = socket(AF_INET, SOCK_STREAM, 0);
	if (lsock < 0)
		quit("socket");

	/* Setear opción reuseaddr... normalmente no es necesario */
	if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("setsockopt");

	sa.sin_family = AF_INET;
	sa.sin_port = htons(4040);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bindear al puerto 4040 TCP, en todas las direcciones disponibles */
	rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0)
		quit("bind");

	/* Setear en modo escucha */
	rc = listen(lsock, 10);
	if (rc < 0)
		quit("listen");

	return lsock;
}

int main()
{
    int lsock;




	
	lsock = mk_lsock();
	wait_for_clients2(lsock);
}
