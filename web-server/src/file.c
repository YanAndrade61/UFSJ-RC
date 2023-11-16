#include "main.h"

/**
 * Loads a file into memory and returns a pointer to the data.
 * 
 * Buffer is not NUL-terminated.
 */
struct file_data *file_load(char *filename)
{
    char *buffer;
    struct stat buf;
    int total_bytes = 0;

    // Get the file size
    if (stat(filename, &buf) == -1) {
        return NULL;
    }

    // Make sure it's a regular file
    if (!(buf.st_mode & S_IFREG)) {
        return NULL;
    }

    // Open the file for reading
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
        return NULL;

    // Allocate that many bytes
    total_bytes = buf.st_size;
    buffer = malloc(total_bytes);

    if (buffer == NULL)
        return NULL;

    // Read in the entire file
    fread(buffer, 1, total_bytes+1, fp);

    // Allocate the file data struct
    struct file_data *filedata = malloc(sizeof *filedata);

    if (filedata == NULL) {
        free(buffer);
        return NULL;
    }

    filedata->data = buffer;
    filedata->size = total_bytes;

    return filedata;
}

/**
 * Frees memory allocated by file_load().
 */

int file_exists (char *filename) {
    struct stat buffer;   
    return stat(filename, &buffer) == 0;
}

void file_free(struct file_data *filedata) {
    free(filedata->data);
    free(filedata);
}

void file_path(char* buff, char* fpath) {
    
    char *token = strtok(buff, " ");
    int count = 1;
    while (token != NULL && count < 2) {
        token = strtok(NULL, " ");
        count++;
    }
    strcat(fpath, RESOURCES_PATH);
    if(strcmp(token,"/") == 0) 
        strcat(fpath,"/index.html");
    else
        strcat(fpath,token);
}

void file_mime(char *filename, char* mimet) {
    char *ext = strrchr(filename, '.');

    if (ext == NULL)
        strcpy(mimet, DEFAULT_MIME_TYPE);

    ext++;

    if (strcmp(ext, "html") == 0 || strcmp(ext, "htm") == 0) strcpy(mimet, "text/html");
    else if (strcmp(ext, "jpeg") == 0 || strcmp(ext, "jpg") == 0) strcpy(mimet, "image/jpg");
    else if (strcmp(ext, "css") == 0) strcpy(mimet, "text/css");
    else if (strcmp(ext, "js") == 0) strcpy(mimet, "application/javascript"); 
    else if (strcmp(ext, "json") == 0) strcpy(mimet, "application/json"); 
    else if (strcmp(ext, "txt") == 0) strcpy(mimet, "text/plain"); 
    else if (strcmp(ext, "gif") == 0) strcpy(mimet, "image/gif"); 
    else if (strcmp(ext, "png") == 0) strcpy(mimet, "image/png"); 
    else strcpy(mimet, DEFAULT_MIME_TYPE);
}