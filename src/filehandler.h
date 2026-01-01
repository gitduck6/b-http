#pragma once

#include <stdlib.h>
#include <stdio.h>

ssize_t sizeOfFile(char * filename);

int fileToBufferN(char * file_name, char * buffer, size_t n);
