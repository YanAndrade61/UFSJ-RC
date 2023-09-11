#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Criação do socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Erro ao criar o socket do cliente");
        exit(EXIT_FAILURE);
    }

    // Configuração do endereço do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Conexão ao servidor
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erro ao conectar ao servidor");
        exit(EXIT_FAILURE);
    }

    // Loop do jogo
    while (1) {
        char buffer[1024];
        int guess;

        // Recebe instruções do servidor
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);

        if (strstr(buffer, "Adivinhe o número") == NULL) {
            break;
        }

        // O jogador faz uma adivinhação
        scanf("%d", &guess);
        sprintf(buffer, "%d", guess);
        send(client_socket, buffer, strlen(buffer), 0);

        // Recebe a resposta do servidor
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);
    }

    // Fecha a conexão
    close(client_socket);

    return 0;
}
