#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BROAD_PORT 8899
#define MAX_MSG 10

int main(int argc, char *argv[]){
    int socket_fd; 
    unsigned int port;
    int ca_size;
    struct sockaddr_in sock_adress, multi_addr; 
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
    /* Crear estructura con una dirección automática */
    if(inet_aton(INADDR_ANY, &sock_adress.sin_addr) == 0){
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
    /* Preparar dirección del multicast*/
    inet_aton(argv[1], &multi_addr.sin_addr);
    multi_addr.sin_family = AF_INET;
    multi_addr.sin_port = htons(atoi(argv[2]));
    /* Activar IP_MULTICAST_IF : Interfaz de salida para el multicast */
    setsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_IF, &multi_addr, sizeof(multi_addr));
    /* Activar IP_MULTICAST_LOOP : Recibir paquetes enviados por el mismo servidor*/
    setsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_IF, &activate, sizeof(activate));
    /* Enviar mensaje MAX_MSG veces */
    for(int i = 0; i < MAX_MSG; i++){
        if(sendto(socket_fd, msg, sizeof(msg), 0, (struct sockaddr*) &multi_addr, sizeof(multi_addr)) == -1){
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
