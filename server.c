#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 4242

#define BUFFER_SIZE 4096

int main() {

    struct sockaddr_in client, server;

    int server_fd, client_fd;

    char buffer[BUFFER_SIZE];

    fprintf(stdout, "Initializing server\n");

    // Creates a TCP socket (SOCK_STREAM) and IPv4 adress (AF_INET)
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating the server socket: ");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Server socket created with fd: %d\n", server_fd);

    // Configure sockets struct
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    memset(server.sin_zero, 0x0, 8);

    // Associate the socket struct to a fd
    int ok = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &ok, 
                    sizeof(int)) == -1) {
        perror("Error with socket options");
        return EXIT_FAILURE;
    }

    // Guarantee exclusive acess to a port
    if(bind(server_fd, (struct sockaddr*)&server,
                sizeof(server)) == -1) {
        perror("Error binding socket: ");
        return EXIT_FAILURE;
    }
    
    // Enter in listen mode and wait connections
    if(listen(server_fd, 1) == -1) {
        perror("Error listening: ");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Listening on port: %d\n", PORT);

    // Estabilish a new connetion and continue execution
    socklen_t client_len = sizeof(client);
    if((client_fd == accept(server_fd, (struct sockaddr*)&client, 
                            &client_len)) == -1) {
        perror("Error acepting:");
        return EXIT_FAILURE;
    }

    strcpy(buffer, "Welcome, player!\n\0");
  
    if(send(client_fd,buffer,strlen(buffer),0)){
        fprintf(stdout, "Client connected...\n");

        // Comunicates with the client
        do {

            memset(buffer, 0x0, BUFFER_SIZE);

            // Receive client message
            int msg_len;
            if((msg_len = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
                buffer[msg_len - 1] = '\0';
                printf("Client says: %s\n", buffer);
            }

            if(strcmp(buffer, "bye") == 0) {
                send(client_fd, "bye", 3, 0);
            } else {
                send(client_fd, "yep\n", 4, 0);
            }

        } while (strcmp(buffer,"bye"));
        
    }

    close(client_fd);
    close(server_fd);

    fprintf(stdout, "Connection closed \n\n");

    return EXIT_SUCCESS;
} 