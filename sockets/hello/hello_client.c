#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    int socket_fd, conn_fd; 
    unsigned int port;
    struct in_addr address; 
    struct sockaddr_in serv_address; 
    char msg[] = "Hola del cliente";
    char buff[32];
    if(argc != 3){
        printf("hello_client <ip> <puerto>\n");
        return -1;
    }
    /* Crear el socket */
    if((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
        printf("Error creando socket ...\n"); 
        return -1;
    }
    /* Crear estructura con dirección del servidor */
    if(inet_aton(argv[1], &address) == 0){
        printf("Formato incorrecto de IP...\n");
        return -1;
    }
    port = atoi(argv[2]);
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(port);
    serv_address.sin_addr = address;
    /* Conectarse al servidor con dir. serv_address */
    if(connect(socket_fd, (struct sockaddr *) &serv_address, sizeof(serv_address)) == -1){
        printf("Error conectandose al servidor...\n");
        return -1;
    }
    /* Enviar mensaje */
    if(send(socket_fd, msg, sizeof(msg), 0) == -1){
        printf("Error enviando msg...\n");
        return -1;
    }
    /* Leer mensaje */ 
    if(recv(socket_fd, buff, sizeof(buff), 0) == -1){
        printf("Error recibiendo msg...\n");
        return -1;
    }
    printf("\tmsg: %s\n", buff);
    /* Eliminar socket */
    close(socket_fd);
    return 0;
}
