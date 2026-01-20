#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>

#include "ioheader.h"
#include "confhandler.h"


#define PORT 8080

#define PATH_LIMIT 256
#define FILE_ROOT "www"
#define DEFAULT_ROUTE "index.html"

#define STATUS_OK 200
#define BAD_REQUEST 400

int handle_client(int server_fd);