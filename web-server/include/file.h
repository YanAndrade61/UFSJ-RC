#ifndef __FILE_H__
#define __FILE_H__

#define DEFAULT_MIME_TYPE "application/octet-stream"

struct file_data {
    int size;
    void *data;
};

struct file_data *file_load(char *filename);
void file_free(struct file_data *filedata);
int file_exists (char *filename);
void file_path(char* buff, char* fpath);
void file_mime(char *filename, char* mimet);

#endif