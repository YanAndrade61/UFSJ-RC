#include "main.h"

#define PORT 8080

int get_argument(int argc, char** argv){
    int n = 0;
    if(argc == 2) n = atoi(argv[1]); 
    return n;
}

int main(int argc, char** argv)
{
    int sockfd, nclient;
    struct sockaddr_in servaddr;
    if(!(nclient = get_argument(argc,argv))){
        printf("Failed: error in argument\nExecute: ./server <num_client>\n");
        exit(EXIT_FAILURE);
    }
    int connfd[nclient], clientsfd[nclient];
    struct sockaddr_in client[nclient];


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed: socket creation\n");
        exit(EXIT_FAILURE);
    }
    bzero(&servaddr, sizeof(servaddr));
   
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
   
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Failed: socket bind\n");
        exit(EXIT_FAILURE);
    }
   
    if ((listen(sockfd, 10)) != 0) {
        printf("Failed: socket listen\n");
        exit(EXIT_FAILURE);
    }

    printf("Waiting players..\n");
	for(int i = 0; i< nclient; i++){
        socklen_t clientsz = sizeof(client[i]);
        connfd[i] = accept(sockfd, (struct sockaddr*)&(client[i]), &clientsz);
        if (connfd[i] < 0) {
            printf("Failed: server accept\n");
            exit(EXIT_FAILURE);
        }
        clientsfd[i] = connfd[i];
        printf("Player %d connection sucessfull...%d\n",i,clientsfd[i]);
    }

    game_server(clientsfd,nclient);

	for(int i = 0; i< nclient; i++)
        close(clientsfd[i]);
    close(sockfd);
}