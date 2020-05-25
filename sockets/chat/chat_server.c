#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BACKLOG 10
#define NUM_CLIENTS 3

void *chat_fun(void*);

int main(int argc, char *argv[]){
    int socket_fd, i, **fd_ptr;
    int fd_clients[NUM_CLIENTS];
    pthread_t tids[NUM_CLIENTS];
    int client = 0; 
    unsigned int port;
    int ca_size;
    struct sockaddr_in sock_adress; 
    //char msg[] = "Hola del servidor";
    if(argc != 3){
        printf("server <ip> <puerto>\n");
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
    printf("Corriendo en %s:%d...\n esperando conexiones\n", argv[1], port);
    fd_ptr = (int**) calloc(1, sizeof(int*));
    *fd_ptr = (int*) calloc(1, sizeof(int));
    for(;;){ 
        /* Aceptar conexión */
        if((**fd_ptr = accept(socket_fd, NULL, NULL)) == -1){
            printf("Error aceptando conexión...\n");
            close(socket_fd);
            return -1;
        }else{
            printf("Nuevo cliente...\n", client);
            pthread_create(&tids[i], NULL, chat_fun, (void *) *fd_ptr);
            *fd_ptr = (int*) calloc(1, sizeof(int));
            client++;
        }
    }
    close(socket_fd);
    return 0;
}

void *chat_fun(void *arg){
    char buff[32];
    int *conn_fd = (int*) arg;
    for(;;){
        /* Leer mensaje */ 
        if(recv(*conn_fd, buff, sizeof(buff), 0) > 0){
            if(buff[0] == 'e' && buff[1] == 'x' && buff[2] == 'i' && buff[3] == 't'){
                close(*conn_fd);
                free(conn_fd);
                return 0;
            }
            printf("%s\n", buff);
        }
    }
}
