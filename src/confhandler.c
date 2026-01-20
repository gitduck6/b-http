#include "confhandler.h"

int config_init(Conf * config_pointer)
{
    size_t buffer_size = 256;
    char * conf_buffer = malloc(buffer_size);

    if (conf_buffer == NULL) {
        perror("Memory allocation issue");
        return 1;
    }

    FILE * fp = fopen(CONF_FILE,"rb");
    if (fp == NULL)
    {
        // IF we couldnt open the file, we put a basic config file in its place
        config_pointer->port = DEFAULT_PORT;
        config_pointer->webroot = DEFAULT_WEBROOT;
        return 1;
    }

    int c;
    int i;
    for (i = 0;(c = fgetc(fp)) != EOF;i++)
    {
        if ((i + 1) >= buffer_size)
        {
            buffer_size *= 2;
            char * temp = realloc(conf_buffer,buffer_size);
            if (temp == NULL)
            {
                perror("Realloc error\n");
                free(conf_buffer);
                return -1;
            }
            conf_buffer = temp;

        }
        conf_buffer[i] = (char)c;
    }
    conf_buffer[i] = '\0';

    fclose(fp);

    char *portPtr = strstr(conf_buffer,"PORT");

    if (portPtr)
    sscanf(portPtr,"PORT=%d;",&(config_pointer->port));
    else 
    config_pointer->port = DEFAULT_PORT;


    char *webrootPtr = strstr(conf_buffer,"WEBROOT");

    if (webrootPtr)
    sscanf(webrootPtr,"WEBROOT=%s255;",(config_pointer->webroot)); 
    else
    config_pointer->webroot = DEFAULT_WEBROOT;

    free(conf_buffer);

    return 0;

}
