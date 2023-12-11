#include "main.h"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int get_argument(int argc, char** argv, char* host, int* port, char* fpath, int* buffer_sz){

    if(argc == 5) {
        strcpy(host, argv[1]);
        *port = atoi(argv[2]);
        strcpy(fpath, argv[3]);
        *buffer_sz = atoi(argv[4]);
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {


    int sockfd, port, buffer_sz;
    char host[BUFFER_SIZE], fpath[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_sz = sizeof(client_addr);

    if (get_argument(argc,argv, host, &port, fpath, &buffer_sz))
        error("Failed: error in argument\nExecute: ./server <host_do_servidor> <porta_servidor> <nome_arquivo> <tam_buffer>\n");

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error("Failedd: socket creation");

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)
        error("Erro ao conectar");

    write(sockfd, fpath, strlen(fpath)+1);

    FILE *fp = fopen("imagem-test.jpg", "w+");
    if (fp == NULL)
        error("Erro ao abrir o arquivo");

    int bytes_received = 0;
    struct timeval start, end;
    gettimeofday(&start, NULL);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(sockfd, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, bytes_read, fp);
        bytes_received += bytes_read;
    }

    fclose(fp);
    close(sockfd);

    gettimeofday(&end, NULL);

    double time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Buffer = %5d byte(s), %10.2f kbps (u bytes em %3u.%06u s)\n",
           buffer_sz, (bytes_received / time) / 1024, (unsigned int)time, (unsigned int)(time * 1000000));

    return 0;
}
