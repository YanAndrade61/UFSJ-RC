#ifndef __CLIENT_H__
#define __CLIENT_H__

void                   error(char *msg);
void            clients_fork(int sockfd);
void           client_thread(int sockfd, struct shared_data* sharedData);
void          clients_select(int sockfd);
void     clients_interactive(int sockfd);

#endif