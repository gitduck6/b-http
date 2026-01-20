#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char * lookup_ext(char * filename);
char * lookup_mime(char * file_extention);
char filetype(const char * filename);

typedef struct content
{
    size_t lenght;  
    size_t buffer_size;
    char * data;
}
Content;

int AddString(Content * data,char * toBeAdded);