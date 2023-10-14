#ifndef __GAME_H__
#define __GAME_H__

#define card2int(c) isdigit(c) ? c-'0' : c == 'A'? 1 : 10

typedef struct player{

    int sum;
    int ncards;
    char cards[21];

    int playerfd;
    int serverfd;

}player;

char read_option(player p);
void send_option(int serverfd, char op);
void receive_cards(player* p, int n);
void send_cards(player* p, int n);
void result_view(player p);
void replay_menu(player p, char* opc);
void player_menu(player p, char* opc);
void game_client (int sockfd);
void game_server(int clientfd[], int n);

#endif