#include "main.h"

#define PORT 8080
   
int get_argument(int argc, char** argv, char serverip[]){
    int n = 0;
    if(argc == 2) {n=1; strcpy(serverip,argv[1]); } 
    return n;
}

int main(int argc, char** argv) {

    int sockfd, connfd;
    char serverip[MAXBUFF];
    struct sockaddr_in servaddr, client;
    if(!get_argument(argc,argv,serverip)){
        printf("Failed: error in argument\nExecute: ./client <server_ip>\n");
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed: socket creation\n");
        exit(EXIT_FAILURE);
    }
    bzero(&servaddr, sizeof(servaddr));
   
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(serverip);
    servaddr.sin_port = htons(PORT);
   
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Failed: server connection\n");
        exit(EXIT_FAILURE);
    }

    game_client(sockfd);

    close(sockfd);
}