#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char * lookup_ext(char * filename);
char * lookup_mime(char * file_extention);
char filetype(char * filename);