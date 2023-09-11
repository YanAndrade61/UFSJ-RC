#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main() {
    int server_socket, player1_socket, player2_socket;
    struct sockaddr_in server_addr, player1_addr, player2_addr;
    socklen_t player1_addr_len = sizeof(player1_addr);
    socklen_t player2_addr_len = sizeof(player2_addr);

    // Criação do socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Erro ao criar o socket do servidor");
        exit(EXIT_FAILURE);
    }

    // Configuração do endereço do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Vincula o socket à porta e ao endereço
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erro ao vincular o socket do servidor");
        exit(EXIT_FAILURE);
    }

    // Coloca o servidor no modo de escuta
    if (listen(server_socket, 2) == -1) {
        perror("Erro ao colocar o servidor no modo de escuta");
        exit(EXIT_FAILURE);
    }

    printf("Aguardando a entrada de dois jogadores...\n");

    // Aceita a entrada dos jogadores
    player1_socket = accept(server_socket, (struct sockaddr *)&player1_addr, &player1_addr_len);
    printf("Jogador 1 conectado\n");
    send(player1_socket, "Você é o Jogador 1. Aguarde o Jogador 2.", strlen("Você é o Jogador 1. Aguarde o Jogador 2."), 0);

    player2_socket = accept(server_socket, (struct sockaddr *)&player2_addr, &player2_addr_len);
    printf("Jogador 2 conectado\n");
    send(player2_socket, "Você é o Jogador 2. O jogo está começando!", strlen("Você é o Jogador 2. O jogo está começando!"), 0);

    // Geração de um número aleatório
    int numero_secreto = rand() % 100 + 1;
    int jogador_atual = 1;  // Começa com o jogador 1

    // Loop principal do jogo
    while (1) {
        char buffer[1024];
        int guess;
        memset(buffer,0x0,1024);

        // Recebe adivinhação do jogador atual
        if (jogador_atual == 1) {
            recv(player1_socket, buffer, sizeof(buffer), 0);
            guess = atoi(buffer);
        } else {
            recv(player2_socket, buffer, sizeof(buffer), 0);
            guess = atoi(buffer);
        }

        // Verifica a adivinhação
        if (guess == numero_secreto) {
            if (jogador_atual == 1) {
                send(player1_socket, "Correto! Você venceu!", strlen("Correto! Você venceu!"), 0);
                send(player2_socket, "Você perdeu. O Jogador 1 venceu!", strlen("Você perdeu. O Jogador 1 venceu!"), 0);
            } else {
                send(player2_socket, "Correto! Você venceu!", strlen("Correto! Você venceu!"), 0);
                send(player1_socket, "Você perdeu. O Jogador 2 venceu!", strlen("Você perdeu. O Jogador 2 venceu!"), 0);
            }
            break;
        } else if (guess < numero_secreto) {
            if (jogador_atual == 1) {
                send(player1_socket, "Tente um número maior.", strlen("Tente um número maior."), 0);
                send(player2_socket, "É a vez do Jogador 2.", strlen("É a vez do Jogador 2."), 0);
                jogador_atual = 2;
            } else {
                send(player2_socket, "Tente um número maior.", strlen("Tente um número maior."), 0);
                send(player1_socket, "É a vez do Jogador 1.", strlen("É a vez do Jogador 1."), 0);
                jogador_atual = 1;
            }
        } else {
            if (jogador_atual == 1) {
                send(player1_socket, "Tente um número menor.", strlen("Tente um número menor."), 0);
                send(player2_socket, "É a vez do Jogador 2.", strlen("É a vez do Jogador 2."), 0);
                jogador_atual = 2;
            } else {
                send(player2_socket, "Tente um número menor.", strlen("Tente um número menor."), 0);
                send(player1_socket, "É a vez do Jogador 1.", strlen("É a vez do Jogador 1."), 0);
                jogador_atual = 1;
            }
        }
    }

    // Fecha as conexões
    close(server_socket);
    close(player1_socket);
    close(player2_socket);

    return 0;
}
