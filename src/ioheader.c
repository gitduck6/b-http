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
    else if (!(strcmp(file_extention,"mp3"))) return "audio/mpeg";
    else if (!(strcmp(file_extention,"mp4"))) return "video/mp4";

    //Scripts
    else if (!(strcmp(file_extention,"php"))) return "script/php";
    else if (!(strcmp(file_extention,"py"))) return "script/python";

    else return "text/plain";

}

char filetype(const char * filename)
{
    struct stat st;
    if (stat(filename, &st) != 0)
    {
        return '?';
        //return ? if it does not exist or an error
    }

    if (S_ISDIR(st.st_mode))
    {
        return 'd';
        // d if IS dir,
    }
    if (S_ISREG(st.st_mode))
    {
        return '-';
        // - if its something else idk
    }
    return '?';

}

int AddString(Content * data,char * toBeAdded)
{
    if (data->buffer_size <= (data->lenght + strlen(toBeAdded)))
    {
        data->buffer_size += strlen(toBeAdded) * 2;
        char * temp = realloc(data->data,data->buffer_size);
        if (temp == NULL)
        {
            perror("Realloc error\n");
            return 0;
        }
        data->data = temp;
    }
    strncpy( data->data + data->lenght, toBeAdded, strlen(toBeAdded) );
    data->data[ data->lenght + strlen(toBeAdded) ] = '\0';
    data->lenght += strlen(toBeAdded);
    // do not question my code pls, i cant even read it myself
}
