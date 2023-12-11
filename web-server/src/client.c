#include "main.h"

void error(char *msg) 
{
    perror(msg);
    exit(1);
}

void clients_interactive(int sockfd) 
{
    struct sockaddr_in client;
    socklen_t clientsz = sizeof(client);
    while (1) {
        int connfd = accept(sockfd, (struct sockaddr *)&(client), &clientsz);
        if (connfd < 0)
            error("Failed: server accept\n");
        web_server(connfd);
        close(connfd);
    }
}

void clients_fork(int sockfd) 
{
    struct sockaddr_in client;
    socklen_t clientsz = sizeof(client);
    while(1) {
        int connfd = accept(sockfd, (struct sockaddr *)&(client), &clientsz);
        if (connfd < 0)
            error("Failed: server accept\n");
        
        pid_t pid = fork();

        if (pid == -1) {
            close(connfd);
            error("Erro ao criar processo filho");
        }
        else if (pid == 0) {           
            web_server(connfd);
            exit(EXIT_SUCCESS);
        }
        else {
            close(connfd); 
        }
    }
}

void clients_select(int sockfd) 
{
    fd_set master;
    fd_set read_fds;
    int fdmax;
    struct sockaddr_in client;
    socklen_t clientsz = sizeof(client);
    
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(sockfd, &master);
    fdmax = sockfd;

    while(1) {
        read_fds = master;

        if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
            error("Failed: Server-select()");

        for(int i = 0; i <= fdmax; i++) {
            if(FD_ISSET(i, &read_fds)) {
                if(i == sockfd) {
                    clientsz = sizeof(client);
                    int connfd = accept(sockfd, (struct sockaddr *)&client, &clientsz);
                    if( connfd < 0){
                        error("Failed: server accept\n");
                    } else {
                        FD_SET(connfd, &master); 
                        if(connfd > fdmax)
                            fdmax = connfd;
                    }
                }
                else {
                    if(web_server(i) == 1) {
                        close(i);
                        FD_CLR(i, &master);
                    }
                }
            }
        }
    }
}

void client_thread(int sockfd, struct shared_data *datash) {
    struct sockaddr_in client;
    socklen_t clientsz = sizeof(client);
    while(1) {
        int connfd = accept(sockfd, (struct sockaddr *)&(client), &clientsz);
        if (connfd < 0)
            error("Failed: server accept\n");
        printf("conn: %d\n",connfd);
        // Adiciona a tarefa à fila
        push_task(datash, connfd);printf("conn: %d\n",connfd);
    }
}