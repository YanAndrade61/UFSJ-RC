#include "main.h"

void web_server(int connfd) {

    char buff[MAXBUFF], fpath[MAXBUFF];
    char response[MAXBUFF], fsize[MAXBUFF];
    char mimet[MAXBUFF];

    bzero(buff, MAXBUFF);
    bzero(fpath, MAXBUFF);
    bzero(response, MAXBUFF);
    bzero(mimet, MAXBUFF);
    bzero(fsize, MAXBUFF);

    if(read(connfd, buff, sizeof(buff)) <= 0) return;

    //Get path requested
    file_path(buff,fpath);
    if(!file_exists(fpath)){
        bzero(fpath, MAXBUFF);
        strcpy(fpath, "./resources/404.html");
        strcat(response, "HTTP/1.1 404 NOT FOUND\r\n");
    }
    else {
        strcat(response, "HTTP/1.1 200 OK\r\n");
    }
    // printf("Path request: %s\n",fpath);
    struct file_data *f = file_load(fpath);
    file_mime(fpath, mimet);

    strcat(response,"Content-Length: ");
    sprintf(fsize, "%d", f->size);
    strcat(response, fsize);
    strcat(response, "\r\nContent-Type: ");
    strcat(response, mimet);
    strcat(response, "\r\nConnection: Closed\r\n\r\n");
    // printf("%s",response);
    write(connfd, response, strlen(response));
    // printf("%s",f->data);
    write(connfd, f->data, f->size);

    printf("Client %d\n%s\n", connfd, response);
}