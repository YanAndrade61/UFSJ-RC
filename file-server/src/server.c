#include "main.h"

void error(char *msg) {
    perror(msg);
    exit(1);
}

int get_argument(int argc, char** argv, int* port, int* buffer_sz){

    if(argc == 3) {
        *port = atoi(argv[1]);   
        *buffer_sz = atoi(argv[2]);
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {

    int sockfd, port, buffer_sz;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_sz = sizeof(client_addr);

    if(get_argument(argc, argv, &port, &buffer_sz))
        error("Failed: error in argument\nExecute: ./server <port> <tam_buffer>\n");


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        error("Failed: socket creation\n");
    }
    bzero(&server_addr, sizeof(server_addr));
   
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
   
    if ((bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) != 0)
        error("Failed: socket bind\n");
   
    if ((listen(sockfd, 10)) != 0)
        error("Failed: socket listen\n");

    printf("Aguardando por conexões na porta %d...\n", port);

    int connfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_sz);
    if (connfd < 0)
        error("Failed: accept connection\n");

    char fpath[BUFFER_SIZE];
    bzero(fpath, sizeof(fpath));
    read(connfd, fpath, sizeof(fpath));

    FILE *fp = fopen(fpath, "rb");
    if (fp == NULL) {
        perror("Failed: open file");
        close(connfd);
        close(sockfd);
        exit(1);
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, buffer_sz, fp)) > 0) {
        write(connfd, buffer, bytes_read);
        bzero(fpath, sizeof(fpath));
    }

    fclose(fp);
    close(connfd);
    close(sockfd);

    gettimeofday(&end, NULL);

    double time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Tempo gasto: %3u.%06u s\n", (unsigned int)time, (unsigned int)(time * 1000000));

    return 0;
}
