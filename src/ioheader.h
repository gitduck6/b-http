#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_LIMIT 256
#define FILE_ROOT "www"

#define STATUS_OK 200
#define BAD_REQUEST 400

char * lookup_ext(char * filename);
char * lookup_mime(char * file_extention);