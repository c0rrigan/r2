#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define TRY_RECV 10
int main(int argc, char *argv[]){
    int socket_fd; 
    unsigned int port;
    int ca_size;
    struct sockaddr_in client_addr, multi_addr; 
    char buff[32];
    if(argc != 3){
        printf("client <ip> <port>\n");
        return -1;
    }
    /* Crear el socket */
    if((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        printf("Error creando socket ...\n"); 
        return -1;
    }
    /* Preparar dirección del multicast*/
    inet_aton(argv[1], &multi_addr.sin_addr);
    multi_addr.sin_family = AF_INET;
    multi_addr.sin_port = htons(atoi(argv[2]));
    /* Activar IP_ADD_MEMBERSHIP : Darse de alta en el multicast */
    setsockopt(socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multi_addr, sizeof(multi_addr));
    /* Asignar puerto y dirección al socket */
    if(bind(socket_fd, (struct sockaddr*) &client_addr, sizeof(client_addr)) == -1){
        printf("Error asignando puerto / dirección a socket... \n");
        close(socket_fd);
        return -1;
    }
    for(int i = 0; i < TRY_RECV; i++){
        if(recvfrom(socket_fd, buff, sizeof(buff), 0, NULL, NULL) == -1){
            printf("Error recibiendo msg...\n");
        }else{
            printf("msg:%s", buff);
        }
        sleep(3);
    }
    /* Eliminar sockets */
    close(socket_fd);
    return 0;
}
