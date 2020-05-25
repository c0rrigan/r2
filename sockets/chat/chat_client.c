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
    printf("Conectandose al servidor...\n");
    if(connect(socket_fd, (struct sockaddr *) &serv_address, sizeof(serv_address)) == -1){
        printf("Error conectandose al servidor...\n");
        return -1;
    }
    printf("Conetado al servidor %s ...\n", inet_ntoa(serv_address.sin_addr));
    for(;;){  
        fgets(buff, sizeof(buff), stdin);
        printf("echo: %s", buff);
        /* Enviar mensaje */
        if(send(socket_fd, buff, sizeof(buff), 0) == -1){
            printf("Error enviando msg...\n");
            return -1;
        }
        if(buff[0] == 'e' && buff[1] == 'x' && buff[2] == 'i' && buff[3] == 't')
            break;
    }
    /* Eliminar socket */
    close(socket_fd);
    return 0;
}
