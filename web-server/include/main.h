#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "web.h"
#include "threadtask.h"
#include "file.h"
#include "client.h"

#define RESOURCES_PATH "./resources"
#define MAXBUFF 1048576
#define MAXCLIENT 10
#define MAXTHREAD 5

#endif