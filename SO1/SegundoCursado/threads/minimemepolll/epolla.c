#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#include <sys/epoll.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>

void quit(char *s)
{
    perror(s);
    abort();
}

#define MAX_MEM 1024
#define MAX_LEN 256

#define MAX_HILOS 10
int epfd;
struct epoll_event ev, events[MAX_HILOS];

struct Node{
    struct Node* sig;
    int fd;
};


struct Queue {
    struct Node* ini;
    struct Node* fin;
    pthread_mutex_t mut;
};
typedef struct Queue* Cola;

Cola crear_cola(){
    Cola cola = malloc(sizeof(struct Queue));
    cola->fin = cola->ini = NULL;
    pthread_mutex_init(&cola->mut, NULL);
    return cola;
}

void enqueue(Cola cola, int fd){
    struct Node* nuevoNodo = malloc(sizeof(struct Node));
    nuevoNodo->fd = fd;
    nuevoNodo->sig = NULL;
    if(cola->ini == NULL){
        cola->ini = nuevoNodo; 
        cola->fin = nuevoNodo;
        return;
    }
    cola->fin->sig = nuevoNodo;  // encadena al final
    cola->fin = nuevoNodo;       // mueve fin al nuevo nodo
    return;
}

int dequeue(Cola cola){
    int ret;
    if(cola->ini == cola->fin){
        if(!cola->ini) return -1;
        else {
            ret = cola->ini->fd;
            free(cola->ini);
            cola->fin = NULL;
            cola->ini = NULL;
            return ret;
        }
    }
    ret = cola->fin->fd;
    struct Node* temp = cola->ini;
    while(temp->sig != cola->fin) temp = temp->sig;
    temp->sig = NULL;
    free(cola->fin);
    cola->fin = temp;
    return ret;
}

Cola cola;

char *mem[MAX_MEM];
pthread_mutex_t arr_mutex[MAX_MEM];

int fd_readline(int fd, char *buf)
{
    int rc;
    int i = 0;

    while ((rc = read(fd, buf + i, 1)) > 0)
    {
        if (buf[i] == '\n')
            break;
        i++;
    }

    if (rc < 0)
        return rc;

    buf[i] = 0;
    return i;
}

char **get_args(char *buffer, int *cantidad)
{
    int cant = 0;
    int max_len = 4;

    int actual_len = 0;
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == ' ')
        {
            cant += 1;
            if (actual_len > max_len)
                max_len = actual_len;
            actual_len = 0;
        }
        actual_len += 1;
    }
    cant += 2;

    (*cantidad) = cant;
    char **args = malloc(sizeof(char *) * cant);

    char *token = strtok(buffer, " ");

    int idx = 0;
    while (token != NULL)
    {
        args[idx] = malloc(sizeof(char) * max_len);
        strcpy(args[idx], token);
        token = strtok(NULL, " ");
        idx++;
    }
    args[idx] = NULL;
    return args;
}

int is_digits_only(const char *s)
{
    if (*s == '\0')
        return 0;
    while (*s)
    {
        if (!isdigit((unsigned char)*s))
            return 0;
        s++;
    }
    return 1;
}
int validate(char **palabras, int cantidad)
{
    if (cantidad > 3 || cantidad <= 1)
        return 0;
    if (strcmp(palabras[0], "PUT") != 0 && strcmp(palabras[0], "DEL") != 0 && strcmp(palabras[0], "GET") != 0)
        return 0;
    if (!is_digits_only(palabras[1]))
        return 0;
    if ((strcmp(palabras[0], "PUT") == 0) && cantidad <= 2)
        return 0;
    return 1;
}

void put_function(unsigned int key, char *valor)
{
    pthread_mutex_lock(&arr_mutex[key]);
    if (mem[key] != NULL)
    {
        free(mem[key]);
    }
    mem[key] = malloc(strlen(valor) + 1);
    strcpy(mem[key], valor);
    pthread_mutex_unlock(&arr_mutex[key]);
}

char *del_function(unsigned int key)
{
    char *ret = "NOT FOUND\n";
    pthread_mutex_lock(&arr_mutex[key]);
    if (mem[key] != NULL)
    {
        free(mem[key]);
        mem[key] = NULL;
        ret = "OK\n";
    }
    pthread_mutex_unlock(&arr_mutex[key]);
    return ret;
}

char *get_function(unsigned int key)
{
    char *ret = NULL;
    pthread_mutex_lock(&arr_mutex[key]);
    if (mem[key])
    {
        ret = malloc(strlen(mem[key]) + 1);
        strcpy(ret, mem[key]);
    }
    pthread_mutex_unlock(&arr_mutex[key]);
    return ret;
}

void *handle_conn(void *arg)
{
    while (1) 
    {
        pthread_mutex_lock(&cola->mut);
        int csock = dequeue(cola);
        pthread_mutex_unlock(&cola->mut);

        if(csock == -1){
            usleep(1000);
            continue;
        }
        char buf[200];
        int rc;
        rc = fd_readline(csock, buf);

        if (rc < 0)
            quit("Error al leer\n");

        if (rc == 0)
        {
            close(csock);
            continue;
        }
        int cantidad = 0;
        buf[strcspn(buf, "\n")] = '\0';
        char **palabras = get_args(buf, &cantidad);
        cantidad--;
        if (validate(palabras, cantidad))
        {
            char reply[MAX_LEN] = "OK\n";
            unsigned int key = atoi(palabras[1]);
            if (key >= MAX_MEM)
            {
                strcpy(reply, "OUT OF RANGE\n");
            }
            else if (!strcmp(palabras[0], "PUT"))
            {
                if (strlen(palabras[2]) > MAX_LEN)
                    strcpy(reply, "Input Overflow\n");
                else
                    put_function(key, palabras[2]);
            }
            else if (!strcmp(palabras[0], "DEL"))
            {
                strcpy(reply, del_function(key));
            }
            else if (!strcmp(palabras[0], "GET"))
            {
                printf("1\n");
                char *ret = get_function(key);
                printf("2\n");
                if (ret == NULL)
                    strcpy(reply, "NOT FOUND\n");
                else
                {
                    printf("3\n");
                    strcpy(reply, "OK ");
                    strcat(reply, ret);
                    strcat(reply, "\n");
                    free(ret);
                }
            }

            write(csock, reply, strlen(reply));
        }
        else if (strcmp(buf, "quit") == 0)
        {
            close(csock);
            for (int i = 0; palabras[i] != NULL; i++) free(palabras[i]);
            free(palabras);   
            continue;
        }
        else
        {
            // Responde servidor EINVAL \n
            char *reply = "EINVAL\n";
            write(csock, reply, strlen(reply));
        }
        for (int i = 0; palabras[i] != NULL; i++) free(palabras[i]);
        free(palabras);

        struct epoll_event event_rearm;
        event_rearm.events = EPOLLIN;
        event_rearm.data.fd = csock;
        epoll_ctl(epfd, EPOLL_CTL_MOD, csock, &event_rearm);
        
    }
    return NULL;
}


int mk_lsock()
{
    struct sockaddr_in sa;
    int lsock;
    int rc;
    int yes = 1;

    lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock < 0)
        quit("SOCKET: Failed\n");

    if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
        quit("SETSOCKOPT: Failed");

    sa.sin_family = AF_INET;
    sa.sin_port = htons(3942);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
    if (rc < 0)
        quit("BIND: Failed\n");

    rc = listen(lsock, 100);
    if (rc < 0)
        quit("LISTEN: Failed\n");

    return lsock;
}

void do_use_fd(int fd) {
    char buffer[1024];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));

    if (bytes_read > 0) {
        printf("Recibido: %.*s\n", (int)bytes_read, buffer);
        write(fd, "OK", 2); // Responder al cliente
    } else if (bytes_read == 0) {
        printf("Cliente desconectado.\n");
        close(fd); // Al cerrar el FD, epoll lo elimina automáticamente
    } else {
        perror("Error en read");
    }
}

int main()
{   
    cola = crear_cola();
    
    for (int i = 0; i < MAX_MEM; i++)
        mem[i] = NULL;
        
    for (int i = 0; i < MAX_MEM; i++)
        pthread_mutex_init(&arr_mutex[i], NULL);
    pthread_t th[4];
    for(int i = 0; i < 4; i++) {
        pthread_create(&th[i], NULL, handle_conn, NULL);
    }

    
    int lsock;
    lsock = mk_lsock();
    
    epfd = epoll_create1(0);
    
    fcntl(lsock, F_SETFL, O_NONBLOCK);

    ev.events = EPOLLIN ;
    ev.data.fd = lsock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lsock, &ev);

    while (1)
    {
        int nfds = epoll_wait(epfd, events, 10, -1);
        for (int i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == lsock)
            {
                int psock = accept(lsock, NULL, NULL);
                ev.events = EPOLLIN | EPOLLONESHOT;
                ev.data.fd = psock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, psock, &ev);
            }
            else
            {
                int socket_cliente = events[i].data.fd;
                pthread_mutex_lock(&cola->mut);
                enqueue(cola,socket_cliente);
                pthread_mutex_unlock(&cola->mut);
            }   
        }
    }
}