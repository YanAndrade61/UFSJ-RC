#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 4242

#define BUFFER_SIZE 4096


int main(int argc, char** argv){

        struct sockaddr_in server;

        int sock_fd, slen, len = sizeof(server);

        char buffer[BUFFER_SIZE];

        fprintf(stdout, "Starting client ...\n");

        if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("Error creating client socket: ");
                return EXIT_FAILURE;
        }
        fprintf(stdout, "Client socket created with fd: %d\n", sock_fd);

        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        server.sin_addr.s_addr = inet_addr(argv[1]);
        memset(server.sin_zero, 0x0, 8);

        if( connect(sock_fd, (struct sockaddr*) &server, len) == -1) {
                perror("Connection to server failed");
                return EXIT_FAILURE;
        }

        if((slen = recv(sock_fd, buffer, BUFFER_SIZE, 0)) > 0) {
                buffer[slen + 1] = '\0';
                fprintf(stdout, "Server: %s", buffer);
        }
        while(1) {

                memset(buffer, 0x0, BUFFER_SIZE);
                fprintf(stdout, "Say something: ");
                fgets(buffer, BUFFER_SIZE, stdin);

                send(sock_fd, buffer, strlen(buffer), 0);

                memset(buffer, 0x0, BUFFER_SIZE);
                slen = recv(sock_fd, buffer, BUFFER_SIZE, 0);
                fprintf(stdout, "Server: ", buffer);

                if(strcmp(buffer, "bye") == 0) break;
        }

        close(sock_fd);
        fprintf(stdout, "\nSee you anytime\n\n");

        return EXIT_SUCCESS;
}