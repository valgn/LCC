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

#define MSG "Hola Fulvio"

int main(){
    int socket_cli = socket(AF_UNIX, SOCK_DGRAM, 0);

    char buff[MAXMSG+1];
    strcpy(buff, MSG);

    struct sockaddr_un srv_name, cli_name;
    srv_name.sun_family = AF_UNIX;
    strcpy(srv_name.sun_path, "SRV_NAME"); 

    cli_name.sun_family = AF_UNIX;
    strcpy(cli_name.sun_path, "SRV_NAME"); 

    unlink(cli_name.sun_path);
    
    socklen_t srv_size = sizeof(srv_name);

    bind(socket_cli, (struct sockaddr *)&cli_name, sizeof(cli_name));
    
    printf("SENT TO..\n");
    
    sendto(socket_cli, buff, 12, 0, (struct sockaddr *)&srv_name, sizeof(srv_name));
    
    printf("[CLI] RECVFROM...\n");
    
    
    int nbytes = recvfrom(socket_cli,buff,MAXMSG, 0, (struct sockaddr *)&srv_name, &srv_size);
    
    printf("[CLI] recv: %s\n",buff);
    
    
    printf("[CLI] SEND OK\n");
    close(socket_cli);
    remove("./CLI_NAME");

    return 0;
}