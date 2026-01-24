#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CONF_FILE "server.conf"

#define DEFAULT_PORT 8080
#define DEFAULT_WEBROOT "www"

typedef struct conf
{
    int port;
    char webroot[256];
}
Conf;

int config_init(Conf * config_pointer);