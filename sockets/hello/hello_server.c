#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BACKLOG 10

int main(int argc, char *argv[]){
    int socket_fd, conn_fd; 
    unsigned int port;
    int ca_size;
    struct sockaddr_in sock_adress, client_address; 
    char msg[] = "Hola del servidor";
    char buff[32];
    if(argc != 3){
        printf("hello_server <ip> <puerto>\n");
        return -1;
    }
    /* Crear el socket */
    if((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
        printf("Error creando socket ...\n"); 
        return -1;
    }
    /* Crear estructura con dirección del servidor */
    if(inet_aton(argv[1], &sock_adress.sin_addr) == 0){
        printf("Formato incorrecto de IP...\n");
        close(socket_fd);
        return -1;
    }
    port = atoi(argv[2]);
    sock_adress.sin_family = AF_INET;
    sock_adress.sin_port = htons(port);
    /* Asignar puerto y dirección al socket */
    if(bind(socket_fd, (struct sockaddr*) &sock_adress, sizeof(sock_adress)) == -1){
        printf("Error asignando puerto / dirección a socket... \n");
        close(socket_fd);
        return -1;
    }
    /* Preparar para aceptar conexiones */ 
    if(listen(socket_fd, BACKLOG) == -1){
        printf("Error preparando socket... \n");
        close(socket_fd);
        return -1;
    }
    printf("Corriendo en %s : %d...\n esperando conexiones\n", argv[1], port);
    ca_size = sizeof(client_address);
    /* Aceptar conexión */
    if((conn_fd = accept(socket_fd, (struct sockaddr*) &client_address, &ca_size)) == -1){
        printf("Error aceptando conexión...\n");
        close(socket_fd);
        return -1;
    }
    printf("Conectado a : %s\n", inet_ntoa(client_address.sin_addr));
    /* Leer mensaje */ 
    if(recv(conn_fd, buff, sizeof(buff), 0) == -1){
        printf("Error recibiendo msg...\n");
        close(socket_fd);
        close(conn_fd);
        return -1;
    }
    printf("\tmsg: %s\n", buff);
    /* Enviar mensaje */
    if(send(conn_fd, msg, sizeof(msg), 0) == -1){
        printf("Error enviando msg...\n");
        close(socket_fd);
        close(conn_fd);
        return -1;
    }
    /* Eliminar sockets */
    close(conn_fd);
    close(socket_fd);
    return 0;
}
