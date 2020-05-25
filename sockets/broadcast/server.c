#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BROAD_PORT 9988
#define MAX_MSG 10

int main(int argc, char *argv[]){
    int socket_fd; 
    unsigned int port;
    int ca_size;
    struct sockaddr_in sock_adress, broad_addr; 
    int activate = 1;
    char msg[] = "Hola del servidor";
    if(argc != 3){
        printf("server <ip> <puerto>\n");
        return -1;
    }
    /* Crear el socket */
    if((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        printf("Error creando socket ...\n"); 
        return -1;
    }
    /* Activar SO_BROADCAST */
    setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &activate, sizeof(activate));
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
    /* Preparar dirección del broadcast*/
    inet_aton("255.255.255.255", &broad_addr.sin_addr);
    broad_addr.sin_family = AF_INET;
    broad_addr.sin_port = htons(BROAD_PORT);
    /* Enviar mensaje MAX_MSG veces */
    for(int i = 0; i < MAX_MSG; i++){
        if(sendto(socket_fd, msg, sizeof(msg), 0, (struct sockaddr*) &broad_addr, sizeof(broad_addr)) == -1){
            printf("Error enviando msg...\n");
        }else{
            printf("Mensaje enviado exitosamente...");
        }
        sleep(3);
    }
    /* Eliminar sockets */
    close(socket_fd);
    return 0;
}
