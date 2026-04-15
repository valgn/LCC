#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h> 


void quit(char *s){
	perror(s);
	abort();
}

#define MAX_MEM 1024
#define MAX_LEN 256

char* mem[MAX_MEM];
pthread_mutex_t arr_mutex[MAX_MEM];

int fd_readline(int fd, char *buf){
	int rc;
	int i = 0;

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

char** get_args(char *buffer, int* cantidad) {
    int cant = 0;
    int max_len = 4;

   
    int actual_len = 0;
    for(int i = 0; buffer[i] != '\0'; i++){
        if (buffer[i] == ' '){
            cant += 1;
            if (actual_len > max_len) max_len = actual_len;
            actual_len = 0;
        }
        actual_len += 1;
    }
    cant += 2;

    (*cantidad) = cant;
    char **args = malloc(sizeof(char*)*cant);

    char *token = strtok(buffer, " "); 
    
    int idx = 0;
    while (token != NULL) {
        args[idx] = malloc(sizeof(char)*max_len);
        strcpy(args[idx], token);
        token = strtok(NULL, " ");
        idx++;
    }
    args[idx] = NULL;
    return args;
}

int is_digits_only(const char *s) {
    if (*s == '\0') return 0; 
    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}
int validate(char** palabras, int cantidad){
    if(cantidad > 3 || cantidad <= 1) return 0;
    if(strcmp(palabras[0], "PUT") != 0 && strcmp(palabras[0], "DEL") != 0 && strcmp(palabras[0], "GET") != 0) return 0;
    if(!is_digits_only(palabras[1])) return 0;
    if((strcmp(palabras[0], "PUT") == 0) && cantidad <= 2) return 0;
    return 1;
}   

void put_function(unsigned int key, char* valor){
    pthread_mutex_lock(&arr_mutex[key]);
    if(mem[key] != NULL){
        free(mem[key]);
    }
    mem[key] = malloc(strlen(valor) + 1);
    strcpy(mem[key], valor);
    pthread_mutex_unlock(&arr_mutex[key]);
}

char* del_function(unsigned int key){
    char* ret = "NOT FOUND\n"; 
    pthread_mutex_lock(&arr_mutex[key]);
    if(mem[key] != NULL){
        free(mem[key]);
        mem[key] = NULL;
        ret = "OK\n"; 
    }
    pthread_mutex_unlock(&arr_mutex[key]);
    return ret;
}


char* get_function(unsigned int key){
    char* ret = NULL;
    pthread_mutex_lock(&arr_mutex[key]);
    if(mem[key]){
        ret = malloc(strlen(mem[key]) + 1);
        strcpy(ret, mem[key]);
    } 
    pthread_mutex_unlock(&arr_mutex[key]);
    return ret;
}


void* handle_conn(void* psock){
    int csock = *(int*)psock;
    free(psock);

	char buf[200];
	int rc;

	while (1) {
		rc = fd_readline(csock, buf);

		if (rc < 0) quit("Error al leer\n");

		if (rc == 0) {
			close(csock);
			return NULL;
		}
        int cantidad = 0;
        buf[strcspn(buf, "\n")] = '\0';
        char** palabras = get_args(buf, &cantidad);
        cantidad--;
		if (validate(palabras, cantidad)) {
			char reply[MAX_LEN] = "OK\n";
            unsigned int key = atoi(palabras[1]);
            if(key >= MAX_MEM){
                strcpy(reply, "OUT OF RANGE\n");
            }
            else if(!strcmp(palabras[0], "PUT")){
                if(strlen(palabras[2]) > MAX_LEN) strcpy(reply, "Input Overflow\n");
                else put_function(key, palabras[2]);
            }
            else if(!strcmp(palabras[0], "DEL")){
                strcpy(reply, del_function(key));
            }
            else if(!strcmp(palabras[0], "GET")){
                char* ret = get_function(key);
                if(ret == NULL) strcpy(reply, "NOT FOUND\n");
                else{
                    strcpy(reply, "OK ");
                    strcat(reply, ret);
                    strcat(reply, "\n");
                    free(ret);
                }

            }
            
			write(csock, reply, strlen(reply));

		} else if (strcmp(buf, "quit") == 0) {
			close(csock);
			return NULL;
		}
        else{
            // Responde servidor EINVAL \n

            char* reply = "EINVAL\n";
            write(csock, reply, strlen(reply));
            
        }
        for(int i = 0; palabras[i] != NULL; i++) free(palabras[i]);
        free(palabras);
	}
    return NULL;
}

void wait_for_clients(int lsock){
    while(1){
        int* psock = malloc(sizeof(int));

	    *psock = accept(lsock, NULL, NULL);
	    if (*psock < 0) {
            free(psock);
            quit("Succesfully Connected\n");
        }
        pthread_t hilo;
        pthread_create(&hilo, NULL, handle_conn, (void*)psock);
        pthread_detach(hilo);
    }
}

int mk_lsock()
{
	struct sockaddr_in sa;
	int lsock;
	int rc;
	int yes = 1;

	lsock = socket(AF_INET, SOCK_STREAM, 0);
	if (lsock < 0) quit("SOCKET: Failed\n");

	if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("SETSOCKOPT: Failed");

	sa.sin_family = AF_INET;
	sa.sin_port = htons(3942);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0) quit("BIND: Failed\n");

	rc = listen(lsock, 100);
	if (rc < 0) quit("LISTEN: Failed\n");

	return lsock;
}

int main(){
    for(int i = 0; i < MAX_MEM; i++) mem[i] = NULL;
    for(int i = 0; i < MAX_MEM; i++) pthread_mutex_init(&arr_mutex[i], NULL);
	int lsock;
	lsock = mk_lsock();
	wait_for_clients(lsock);
}