#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define CLI_NOMBRE "./CLIENTE"
#define MSG "FLAVIO!"
#define SRV_NOMBRE "./SRV_NAME"

#define MAXMSG 1024
int main(){
        int sock_cli;
        struct sockaddr_un srv_name, cli_name;
        socklen_t srv_size;
        
        ssize_t nbytes;
        char buff[MAXMSG+1];

        sock_cli = socket(AF_UNIX, SOCK_STREAM,0);
        if(sock_cli <0){
            perror("fallo la conexion del oscket");
            exit(EXIT_FAILURE);

        }

        printf("[DIAG] Se creo el socket :D\n");

        cli_name.sun_family = AF_UNIX;
        strncpy(cli_name.sun_path, CLI_NOMBRE, sizeof(cli_name.sun_path));

        unlink(cli_name.sun_path);

        srv_name.sun_family = AF_UNIX;
        strcpy(srv_name.sun_path, SRV_NOMBRE);  
        srv_size = sizeof(struct sockaddr_un);

        connect(sock_cli, (struct sockaddr *)&srv_name, srv_size);

        printf("[DIAG] Conexion con %s OK!\n", SRV_NOMBRE);

        send(sock_cli, MSG, strlen(MSG), 0);

        nbytes = recv(sock_cli, buff, MAXMSG,0);

        buff[nbytes] = '\0';

        close(sock_cli);
        
        


    return 0;
}