#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

typedef struct
{
    char * content;
    size_t size;
    size_t len;
}
string;

ssize_t sizeOfFile(char * filename);

string loadFile(char * filename);
