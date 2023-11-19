#include "main.h"

int get_argument(int argc, char **argv, int* op) {
    int port = 0;
    if (argc == 3) {
        port = atoi(argv[1]);
        *op = atoi(argv[2]);
    }
    return port;
}

int main(int argc, char **argv) {
    int sockfd, port, op;
    struct sockaddr_in servaddr;
    if (!(port = get_argument(argc, argv,&op)))
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

    switch (op){
        case 1:
            clients_interactive(sockfd);
            break;
        case 2:
            clients_fork(sockfd);
            break;
        case 3:
            clients_select(sockfd);
            break;
        case 4:
            struct shared_data datash;
            datash.head = datash.tail = -1;
            pthread_mutex_init(&(datash.mutex), NULL);

            pthread_t threads[MAXTHREAD];
            for (int i = 0; i < MAXTHREAD; i++) {
                if (pthread_create(&threads[i], NULL, thread_function, (void*)&datash) != 0) {
                    close(sockfd);
                    error("Failed: thread creation");
                }
            }
            client_thread(sockfd, &datash);
        default:
            break;
    }

    close(sockfd);
}