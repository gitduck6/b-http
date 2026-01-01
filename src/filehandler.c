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

int fileToBufferN(char * file_name, char * buffer, size_t buffer_size)
{
    FILE * fp = fopen(file_name,"rb");
    if (!fp)
    {
        perror("fopen failed\n");
        return -1;
    }
    int c;
    size_t i;
    for (i = 0;(i < buffer_size);i++)
    {
        if ((c = fgetc(fp)) != EOF)
        {
            buffer[i] = (char)c;
        }
        else 
        {
            buffer[i] = '\0';
            return i;
        }
    }
    buffer[i] = '\0';
    return buffer_size;
}

int main(void)
{
    char buffer[5000];
    fileToBufferN("main.c",buffer,sizeof(buffer));
    printf("%s",buffer);
    return 0;
}