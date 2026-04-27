#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h> // Necesario para EAGAIN y EWOULDBLOCK
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#define MAX_MEM 1024
#define MAX_LEN 256
#define MAX_EVENTS 10
#define NUM_THREADS 4
#define PORT 3942

char *mem[MAX_MEM];
pthread_mutex_t arr_mutex[MAX_MEM];
int epoll_fd;

void quit(char *s) {
    perror(s);
    abort();
}

void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return;
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// Extrae argumentos de forma segura asignando memoria dinámica exacta
char **get_args(char *buffer, int *cantidad) {
    int cant = 1;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ' ') cant++;
    }
    cant++; // Espacio para el puntero NULL final

    *cantidad = 0;
    char **args = malloc(sizeof(char *) * cant);
    if (!args) return NULL;

    char *token = strtok(buffer, " ");
    while (token != NULL) {
        args[*cantidad] = malloc(strlen(token) + 1);
        strcpy(args[*cantidad], token);
        (*cantidad)++;
        token = strtok(NULL, " ");
    }
    args[*cantidad] = NULL;
    return args;
}

int is_digits_only(const char *s) {
    if (!s || *s == '\0') return 0;
    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

int validate(char **palabras, int cantidad) {
    if (cantidad > 3 || cantidad <= 1) return 0;
    if (!palabras[0] || !palabras[1]) return 0;
    
    if (strcmp(palabras[0], "PUT") != 0 && 
        strcmp(palabras[0], "DEL") != 0 && 
        strcmp(palabras[0], "GET") != 0) return 0;
        
    if (!is_digits_only(palabras[1])) return 0;
    if ((strcmp(palabras[0], "PUT") == 0) && cantidad < 3) return 0;
    return 1;
}

void put_function(unsigned int key, char *valor) {
    pthread_mutex_lock(&arr_mutex[key]);
    if (mem[key] != NULL) {
        free(mem[key]);
    }
    mem[key] = malloc(strlen(valor) + 1);
    strcpy(mem[key], valor);
    pthread_mutex_unlock(&arr_mutex[key]);
}

char *del_function(unsigned int key) {
    char *ret = "NOT FOUND\n";
    pthread_mutex_lock(&arr_mutex[key]);
    if (mem[key] != NULL) {
        free(mem[key]);
        mem[key] = NULL;
        ret = "OK\n";
    }
    pthread_mutex_unlock(&arr_mutex[key]);
    return ret;
}

char *get_function(unsigned int key) {
    char *ret = NULL;
    pthread_mutex_lock(&arr_mutex[key]);
    if (mem[key]) {
        ret = malloc(strlen(mem[key]) + 1);
        strcpy(ret, mem[key]);
    }
    pthread_mutex_unlock(&arr_mutex[key]);
    return ret;
}

// Nueva versión asíncrona: Lee un bloque, responde y rearma el socket
void handle_conn(int csock) {
    char buf[MAX_LEN * 2]; // Buffer suficientemente grande
    int rc = read(csock, buf, sizeof(buf) - 1);

    if (rc < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Falsa alarma, rearmamos para seguir escuchando
            struct epoll_event ev;
            ev.events = EPOLLIN | EPOLLONESHOT;
            ev.data.fd = csock;
            epoll_ctl(epoll_fd, EPOLL_CTL_MOD, csock, &ev);
        } else {
            close(csock); // Error real, cerramos conexión
        }
        return;
    }

    if (rc == 0) {
        // El cliente cerró la conexión
        close(csock);
        return;
    }

    // Limpiamos el salto de línea y aseguramos el fin de cadena
    buf[rc] = '\0';
    buf[strcspn(buf, "\r\n")] = '\0'; 

    if (strcmp(buf, "quit") == 0) {
        close(csock);
        return;
    }

    int cantidad = 0;
    char **palabras = get_args(buf, &cantidad);

    if (palabras && validate(palabras, cantidad)) {
        char reply[MAX_LEN + 50] = "OK\n";
        unsigned int key = atoi(palabras[1]);

        if (key >= MAX_MEM) {
            strcpy(reply, "OUT OF RANGE\n");
        } else if (!strcmp(palabras[0], "PUT")) {
            if (strlen(palabras[2]) > MAX_LEN) {
                strcpy(reply, "Input Overflow\n");
            } else {
                put_function(key, palabras[2]);
            }
        } else if (!strcmp(palabras[0], "DEL")) {
            strcpy(reply, del_function(key));
        } else if (!strcmp(palabras[0], "GET")) {
            char *ret = get_function(key);
            if (ret == NULL) {
                strcpy(reply, "NOT FOUND\n");
            } else {
                snprintf(reply, sizeof(reply), "OK %s\n", ret); // Más seguro que strcat
                free(ret);
            }
        }
        write(csock, reply, strlen(reply));
    } else if (strlen(buf) > 0) {
        char *reply = "EINVAL\n";
        write(csock, reply, strlen(reply));
    }

    // Liberar memoria de los argumentos
    if (palabras) {
        for (int i = 0; palabras[i] != NULL; i++) {
            free(palabras[i]);
        }
        free(palabras);
    }

    // IMPORTANTE: Rearmar el socket para que epoll avise del próximo comando
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLONESHOT;
    ev.data.fd = csock;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, csock, &ev) < 0) {
        close(csock); // Si falla el rearmado, cerramos para no dejar un socket zombi
    }
}

int mk_lsock() {
    struct sockaddr_in sa;
    int lsock, rc;
    int yes = 1;

    lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock < 0) quit("SOCKET: Failed\n");

    if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
        quit("SETSOCKOPT: Failed");

    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
    if (rc < 0) quit("BIND: Failed\n");

    rc = listen(lsock, 100);
    if (rc < 0) quit("LISTEN: Failed\n");

    return lsock;
}

void *worker(void *arg) {
    int lsock = *(int *)arg;
    struct epoll_event events[MAX_EVENTS];

    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == lsock) {
                // Loop de accept: procesa todos los clientes en cola
                while (1) {
                    int csock = accept(lsock, NULL, NULL);
                    if (csock == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break; // Ya no hay más clientes en espera
                        }
                        break; // Error ignorado, la próxima iteración lo reintentará
                    }
                    
                    set_nonblocking(csock); // Devolvemos el socket a no-bloqueante
                    
                    struct epoll_event ev_client;
                    ev_client.events = EPOLLIN | EPOLLONESHOT;
                    ev_client.data.fd = csock;
                    
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, csock, &ev_client) == -1) {
                        perror("epoll_ctl: conn_sock");
                        close(csock);
                    }
                }
            } else {
                // Hay datos de un cliente
                handle_conn(events[n].data.fd);
            }
        }
    }
    return NULL;
}

int main() {
    struct epoll_event ev;
    
    // Inicializar memoria y mutexes
    for (int i = 0; i < MAX_MEM; i++) {
        mem[i] = NULL;
        pthread_mutex_init(&arr_mutex[i], NULL);
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) quit("EPOLL_CREATE: Failed\n");

    int lsock = mk_lsock();
    set_nonblocking(lsock); // El socket de escucha DEBE ser no-bloqueante

    // EPOLLEXCLUSIVE evita que todos los hilos se despierten por una misma conexión
    ev.events = EPOLLIN | EPOLLEXCLUSIVE;
    ev.data.fd = lsock;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, lsock, &ev) == -1) {
        quit("EPOLL_CTL: Failed en lsock\n");
    }

    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, &lsock);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Limpieza de mutex (Si el programa alguna vez llegara aquí)
    for (int i = 0; i < MAX_MEM; i++) {
        pthread_mutex_destroy(&arr_mutex[i]);
    }
    close(lsock);
    close(epoll_fd);

    return 0;
}