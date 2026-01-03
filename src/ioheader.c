#include "ioheader.h"

// Example : myfile.png -> .png
char * lookup_ext(char * filename)
{

    int end;
    int pos;

    for (end = 0;filename[end] != '\0';end++);
    for (pos = end - 1;(filename[pos] != '.') && (pos >= 0);pos--);

    return filename + pos + 1;

}
