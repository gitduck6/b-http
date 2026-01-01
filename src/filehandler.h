#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    char * content;
    size_t size;
}
string;

ssize_t sizeOfFile(char * filename);

string loadFile(char * filename);
