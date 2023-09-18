#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080

int main() {
    int serverSocket, clientSocket1, clientSocket2;
    struct sockaddr_in serverAddress, clientAddress1, clientAddress2;
    char buffer[1024];

    // Inicializar o gerador de números aleatórios com o tempo atual
    srand(time(NULL));

    // Criar um soquete para o servidor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Erro ao criar o soquete do servidor");
        exit(EXIT_FAILURE);
    }

    // Configurar informações do endereço do servidor
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Vincular o soquete ao endereço e à porta
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Erro ao vincular o soquete");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Aguardar conexões de dois clientes
    if (listen(serverSocket, 2) == -1) {
        perror("Erro ao aguardar conexões");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Aguardando conexões de dois clientes...\n");

    // Aceitar a primeira conexão de cliente
    socklen_t clientAddressLen1 = sizeof(clientAddress1);
    clientSocket1 = accept(serverSocket, (struct sockaddr *)&clientAddress1, &clientAddressLen1);
    if (clientSocket1 == -1) {
        perror("Erro ao aceitar a primeira conexão do cliente");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }
    printf("Primeiro cliente conectado.\n");

    // Aceitar a segunda conexão de cliente
    socklen_t clientAddressLen2 = sizeof(clientAddress2);
    clientSocket2 = accept(serverSocket, (struct sockaddr *)&clientAddress2, &clientAddressLen2);
    if (clientSocket2 == -1) {
        perror("Erro ao aceitar a segunda conexão do cliente");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }
    printf("Segundo cliente conectado.\n");

    // Lógica do jogo de par ou ímpar
    while (1) {
        int num_cliente1, num_cliente2, start = 1;

        // Receber numero do cliente
        send(clientSocket1, &start, sizeof(start), 0);        
        if (recv(clientSocket1, &num_cliente1, sizeof(num_cliente1), 0) <= 0) {
            perror("Erro ao receber escolha do cliente");
            break;
        }

        send(clientSocket2, &start, sizeof(start), 0);
        if (recv(clientSocket2, &num_cliente2, sizeof(num_cliente2), 0) <= 0) {
            perror("Erro ao receber escolha do cliente");
            break;
        }

        // Determinar o resultado (par ou ímpar)
        int resultado = (num_cliente1+num_cliente2) % 2;

        // Enviar o resultado ao cliente
        send(clientSocket1, &resultado, sizeof(resultado), 0);
        send(clientSocket2, &resultado, sizeof(resultado), 0);

        // Esperar pela escolha do cliente para continuar ou sair
        printf("Aguardando a escolha do cliente para continuar ou 's' para sair...\n");
        char continuar1, continuar2;
        recv(clientSocket1, &continuar1, sizeof(continuar1), 0);
        recv(clientSocket1, &continuar2, sizeof(continuar2), 0);

        if (continuar1 == 's' && continuar1 == continuar2) {
            // O cliente deseja continuar o jogo
            printf("Jogo continuando...\n");
        } else {
            // O cliente deseja sair
            printf("Clientes desconectado.\n");
            break;
        }
    }

    // Fechar soquetes e encerrar o servidor
    close(clientSocket1);
    close(clientSocket2);
    close(serverSocket);

    return 0;
}
