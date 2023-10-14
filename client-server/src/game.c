#include"main.h"

void send_option(int serverfd, char op){
    char buff[MAXBUFF];
    bzero(buff, sizeof(buff));
    buff[0] = op;
    write(serverfd, buff, sizeof(buff));
}
char read_option(player p) {
    char buff[MAXBUFF];
    bzero(buff, MAXBUFF);
    read(p.playerfd, buff, sizeof(buff));
    return buff[0];
}

void send_cards(player* p, int n) {
    char cards[] = {'A','2','3','4','5','6','7','8','9','J','Q','K'};
    char buff[MAXBUFF], c;
    bzero(buff, sizeof(buff));
    for (int i = 0; i <= n; i+=2){    
        c = cards[rand()%12];
        buff[i] = c;
        p->sum += card2int(c);
    }
    write(p->playerfd, buff, sizeof(buff)); 
}

void receive_cards(player* p, int n){
    char buff[MAXBUFF];
    bzero(buff, sizeof(buff));
    read(p->serverfd, buff, sizeof(buff)); 
    for (int i = 0; i <= n; i+=2){    
        p->cards[p->ncards] = buff[i];
        p->sum += card2int(buff[i]);
        p->ncards += 1;
    }
}

void result_view(player p){
    char buff[MAXBUFF];
    bzero(buff, sizeof(buff));
    read(p.serverfd, buff, sizeof(buff));
    if(buff[0] == '1')
        printf("\nYou win the game\n");
    else
        printf("\nYou lose the game\n");
}

void replay_menu(player p, char* opc) {
    int op;
    do {
        printf("(0) Exit\n(1) Play again\nOption number: ");
        scanf("%d",&op);
    } while(op != 0 && op != 1);
    *opc = (char)(op+'0');
    //Send option to server
    send_option(p.serverfd,*opc);
}


void player_menu(player p, char* opc){
    
    int op;
    system("clear");
    printf("Cards in hand:");
    for (int i = 0; i < p.ncards; i++)
        printf(" %c",p.cards[i]);
    printf("\nCurrent sum: %d\n", p.sum);

    do{
        printf("\n(0) Get one more card\n(1) Stop here\nOption number: ");
        scanf("%d",&op);
    } while(op != 0 && op != 1);
    *opc = (char)(op+'0');

    //Send option to server
    send_option(p.serverfd,*opc);
}

void game_client (int sockfd)
{
    player p;
	char op;
    p.serverfd = sockfd;
    do {
        system("clear");
        printf("Blackjack new round\nWait your turn...\n");
        p.ncards = p.sum = 0;
        //Receive first two cards
        receive_cards(&p,2);
        while(1) {

            //Decide if get another card or stop
            player_menu(p, &op);

            if(op == '0'){
                receive_cards(&p,1);
                printf("Card receive: %c\n",p.cards[p.ncards-1]);
            }
            if(op == '1' || p.sum > 21){
                system("clear");
                p.sum > 21 ? printf("EXPLODE!!!\n") : 0;
                printf("Cards in hand:");
                for (int i = 0; i < p.ncards; i++)
                    printf(" %c",p.cards[i]);
                printf("\nFinal sum: %d\nWait for others players result!\n",p.sum);
                break;
            }
        }
        result_view(p);    
        replay_menu(p, &op);
        if(op == '0') break; 

    }while(op == '1');
}

void game_server(int clientfd[], int n) {
    char op;
    player players[n];
    int best21, winners[n], playing[n], pass = n,tie;

    for (int i = 0; i < n; i++) {
        players[i].ncards = 0;
        players[i].playerfd = clientfd[i];
    }
    srand(time(NULL));
    memset(playing,1,n);
    do{
        printf("\nNew blackjack round with %d players\n",pass);
        best21 = -1;
        for (int i = 0; i < n; i++) {
            if(!playing[i]) continue;
            players[i].sum = 0;
            printf("Player %d turn\n",i);
            // Send first 2 cards of player
            send_cards(&players[i],2);
            // While player still wants more card or explode (>21)
            while(1) {
                op = read_option(players[i]);
                if(op == '0')
                    send_cards(&players[i],1);
                if(op == '1' || players[i].sum > 21)
                    break;

            }
            // Store best pontuaction, and possible a tie
            if(players[i].sum <= 21 && players[i].sum > best21){
                memset(winners,0,n);
                best21 = players[i].sum;
                winners[i] = 1;
                tie = 0;
            }
            else if(players[i].sum == best21) {
                winners[i] = 1;
                tie = 1;
            }
        }
        //Send results for all players
        printf("\nBlackjack results\n");
        for (int i = 0; i < n; i++) {
            if(!playing[i]) continue;
            if(winners[i]) send_option(players[i].playerfd,'1');
            else send_option(players[i].playerfd,'0');
            printf("Player %d: %d\n",i, players[i].sum);
        }
        //Verify what players still want to play
        pass = 0;
        for (int i = 0; i < n; i++) {
            if(!playing[i]) continue;
            op = read_option(players[i]);
            playing[i] = op-'0';
            pass += op-'0';
        }

    }while(pass);
}