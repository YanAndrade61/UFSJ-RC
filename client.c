#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

#define IP_ADDR "172.18.190.87"

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[1024];

    // Criar um soquete para o cliente
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Erro ao criar o soquete do cliente");
        exit(EXIT_FAILURE);
    }

    // Configurar informações do endereço do servidor
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr(IP_ADDR);

    // Conectar-se ao servidor
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Erro ao conectar-se ao servidor");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    printf("Conectado ao servidor.\n");

    while (1) {
        int escolha_cliente;
        int numero, start;
        int resultado;

        if (recv(clientSocket, &start, sizeof(start), 0) <= 0) {
            perror("Erro ao receber número do servidor");
            break;
        }

        // Escolher par (0) ou ímpar (1)
        printf("Escolha par (0) ou ímpar (1): ");
        scanf("%d", &escolha_cliente);

        printf("Insira o numero desejado: ");
        scanf("%d", &numero);

        // Enviar a escolha para o servidor
        send(clientSocket, &numero, sizeof(numero), 0);

        // Receber o resultado do servidor (par ou ímpar)
        if (recv(clientSocket, &resultado, sizeof(resultado), 0) <= 0) {
            perror("Erro ao receber resultado do servidor");
            break;
        }

        if (resultado == escolha_cliente) {
            printf("Você venceu!\n");
        } else {
            printf("Você perdeu.\n");
        }

        // Solicitar ao cliente para continuar ou sair
        printf("Digite 's' para sair ou qualquer outra tecla para continuar: ");
        char escolha_continuar;
        getchar(); // Consumir nova linha anterior
        scanf("%c", &escolha_continuar);

        // Enviar escolha ao servidor
        send(clientSocket, &escolha_continuar, sizeof(escolha_continuar), 0);

        if (escolha_continuar == 's') {
            printf("Desconectando do servidor.\n");
            break;
        }
    }

    // Fechar o soquete do cliente
    close(clientSocket);

    return 0;
}
