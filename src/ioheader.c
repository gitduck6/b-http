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

char * lookup_mime(char * file_extention)
{
    char * content_type;

    // Text type
    if (!(strcmp(file_extention,"html"))) return "text/html";
    else if (!(strcmp(file_extention,"htm"))) return "text/html";
    else if (!(strcmp(file_extention,"css"))) return "text/css";
    else if (!(strcmp(file_extention,"js"))) return "text/javascript";

    //Multimedia
    else if (!(strcmp(file_extention,"png"))) return "image/png";
    else if (!(strcmp(file_extention,"jpeg"))) return "image/jpeg";
    else if (!(strcmp(file_extention,"jpg"))) return "image/jpeg";
    else if (!(strcmp(file_extention,"gif"))) return "image/gif";

    else return "text/plain";

}