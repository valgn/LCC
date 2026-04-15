#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define MAXMSG 1024

#define SRV_NOMBRE "./SRV_NAME"

int main(){
    
    struct sockaddr_un srv_name, cli_name;
    socklen_t srv_size;
    socklen_t cli_size;

    ssize_t nbytes;
    char buff[MAXMSG+1];

    int socket_srv =  socket(AF_UNIX,SOCK_STREAM, 0);

    printf("[SRV] SOCKET 0k :D\n");

    srv_name.sun_family = AF_UNIX;
    strcpy(srv_name.sun_path, SRV_NOMBRE);
    
    unlink(srv_name.sun_path);
    
    bind(socket_srv,(struct sockaddr *)&srv_name,sizeof(srv_name));


    listen(socket_srv, 1);

    int socket_cli = accept(socket_srv, (struct sockaddr *)&cli_name, &cli_size);

    

    nbytes = recv(socket_cli, buff, MAXMSG, 0);
    
    printf("[SRV] Llegó >%s<\n",buff);
    
    send(socket_cli, buff, nbytes, 0);

    //buff[nbytes] = '\0';
    printf("[SRV] NOS VEMOS PA\n");
    close(socket_cli);

    //close(socket_srv);

    remove(SRV_NOMBRE);

    return 0;
}