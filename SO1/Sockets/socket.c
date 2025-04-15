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

//socket es como un file descriptor
/*
el protocol va en 0
Local: AF_UNIX, AF_LOCAL || EN LA MISMA COMPU
Red: AF_INET, AF_INET 6 || RED XD

UDP: SOCK_DGRAM
TCP: SOCK_STREAM

Socket orientado a daragrama, pensarlo como un canal. Se usa pra enviar info o recibir. Pensarlo como sistema de correo

SOCK_DGRAM: Datagramas son como cartas y a que compu se la mando (IP) no maneja q la carta llega, no se verifica que llega. ESTO ES UDP, SE PUEDEN
PERDER. No tienen orden tampoco. VEntajas: Es rapido, si llega llega, mando datos. Las camaras son de UDP 

Tienen 32 bits, los primkeros 16 bits son del puerto fuente y lo otro el puerto destino. Length la longitud del mensaje, el checksum 
es para  ver si llegaron bien los datovich

Para poder mandar por red los datagramas hay que agregar la direccion (adress) y el destinatario (IP ) 

TCP: STREAM. Idea conceptual: Llamar por telefono y se dcie con quien se quiere conectar y una vez que se contesta estan los dos conectados
Ahora se manda un flujo de info (bytes) NO se pierden mensajes, se llega en orden, si en algun momento se corta la conexion
los dos se enteran que se corto en la conexion. conexion bidireccional (FULL DUPLEX)

Como se garantiza que los datos llegan, el flujo de datos se divide en paquetes. Cuando se manda un paquete, cuando el receptor lo recibe
se manda un ack (acuso de recibo) y el sender recibe ese ack. Puede ser que los paquetes se pierdan ( time out lo maneja el protocolo)

Este stream tiene una estructura: Puertos, sequence number (orden de los packs), hay un campo reservado para el ACK 
Si quiero mandar un TCP a traves de la red. Se necesita los IP del source address y el destination

Client: send(), recv()
Server: recv(), send()





int socket(int domain, int typ, int protocol){ // DOminio local o red, Typ: datagrama o stream. Int es fd que devuelve el socke 

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen){ /*skcaddr es la direccion a la que bindeamos la dir

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrle

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t *addrlen)


*/



// struct sockaddr_in{
//     sa_family_t sin_family; 
//     in_port_t sin_port;
//     struct_in_addr sin_addr;

// }




int main(){
    int socket_srv = socket(AF_UNIX, SOCK_DGRAM, 0);

    int cli_size;
    char buff[MAXMSG+1];
    
    struct sockaddr_un srv_name, cli_name;
    srv_name.sun_family = AF_UNIX;
    
    strcpy(srv_name.sun_path, "SRV_NAME"); 
    
    unlink(srv_name.sun_path);
    
    bind(socket_srv, (struct sockaddr *)&srv_name, sizeof(srv_name));
    
    printf("[SRV] RECVFROM...\n");

    int nbytes = recvfrom(socket_srv,buff,MAXMSG, 0, (struct sockaddr *)&cli_name, &cli_size);

    buff[nbytes] = '\0';
    
    printf("[SRV] recv: %s\n",buff);

    //sleep(2);

    sendto(socket_srv, buff, 12, 0, (struct sockaddr *)&cli_name, cli_size);

    printf("[SRV] SEND OK\n");

    close(socket_srv);
    remove("./SVR_NAME");

    return 0;
}