#include "filehandler.h"

ssize_t sizeOfFile(char * filename)
{
    ssize_t size = 0;
    FILE * fp = fopen(filename,"rb");
    if (!fp)
    {
        // kind of weird but ill put the errno values opposite in size
        // this way i can later tell the client why it failed
        size = -errno;
        fprintf(stderr,"fopen failed: %d\n",size);
        return size; // (negative value meaning an error)
    }

    fseek(fp,0,SEEK_END);
    size = ftell(fp);

    fclose(fp);
    return size;
}

// a function that loads a file into memory, as a string type
string loadFile(char * filename)
{
    FILE * fp = fopen(filename,"rb");
    if (!fp)
    {
        fprintf(stderr,"Error reading from file %s\n",filename);
        return (string){0};
    }
    string data = {0};
    size_t size = 32;

    data.content = malloc(size);
    if (!data.content)
    {
        fprintf(stderr,"Memory allocation failure\n");
        return (string){0};
    }
    
    char c;
    char * temp;
    for (int i = 0;(c = fgetc(fp)) != EOF;i++)
    {
        data.content[i] = c;
        data.size++;
        if ((data.size + 1) >= size)
        {
            size *= 2;
            temp = realloc(data.content,size);
            if (temp == NULL)
            {
                fprintf(stderr,"Memory allocation failure\n");
                return (string){0};
            }
            data.content = temp;
        }
    }
    fclose(fp);
    return data;
}

int setString(string *str,char * newstr,size_t StrSize)
{
    str->content = newstr;
    str->size = StrSize;
}