#include "ioheader.h"

// Example : myfile.png -> .png
char * lookup_ext(char * filename)
{
    int pos;
    for (pos = 0;filename[pos] != '\0';pos++)
    {
        if (filename[pos] == '.') break;
    }

    return (filename + pos);

}

int main(void)
{
    printf("%s",lookup_ext("TEST.com"));
}