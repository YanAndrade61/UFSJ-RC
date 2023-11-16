#include "main.h"

#define PORT 8080
char *ROOT = "arquivos";

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int get_argument(int argc, char **argv)
{
    int n = 0;
    if (argc == 2)
        n = atoi(argv[1]);
    return n;
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

void clients_fork(int sockfd) {
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

int main(int argc, char **argv)
{
    int sockfd, port;
    struct sockaddr_in servaddr;
    if (!(port = get_argument(argc, argv)))
        error("Failed: error in argument\nExecute: ./server <port>\n");

    int connfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        error("Failed: socket creation\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
        error("Failed: socket bind\n");

    if ((listen(sockfd, 10)) != 0)
        error("Failed: socket listen\n");

    clients_fork(sockfd);

    close(sockfd);
}