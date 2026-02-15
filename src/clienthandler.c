#include "clienthandler.h"
#include "ioheader.h"


int handle_client(int server_fd)
{
    int running = 1;
    while (running)
    {
        /*Accepting the client*/
        int client_fd = accept(server_fd,NULL,NULL);

        if (client_fd < 0)
        {
            perror("Accept error\n");
            continue;
        }

        /*Parsing the request*/
        char user_req[4096];
        size_t total = 0;
        size_t received;

        char path[PATH_LIMIT];
        char full_path[PATH_LIMIT + strlen(parameters.webroot)];

        int status_code;

        while ((received = recv(client_fd,user_req + total,sizeof(user_req) - total - 1,0)) > 0)
        {
            total += received;
            if (strstr(user_req,"\r\n\r\n")) break;
        }
        user_req[total] = '\0';

        printf("Request :\n %s",user_req);

        sscanf(user_req,"GET %255s ",path);
        snprintf(full_path, sizeof(full_path), "%s", parameters.webroot);
        snprintf(full_path + strlen(parameters.webroot),sizeof(full_path),"%s",path);
        // concentrate index.html into www/index.html
        
        if (filetype(full_path) == 'd')
        {
            int len = strlen(full_path);
            snprintf(full_path + len, sizeof(full_path) - len, "%s","/" DEFAULT_ROUTE);
            //If its a folder it turns into path/index.html
        }
        
        // Content-type handling
        char * content_type = lookup_mime(lookup_ext(full_path)); 

        /*Preparing and sending the response*/

        FILE * requested_file = fopen(full_path,"rb");

        Content resource;

        resource.buffer_size = 64;
        resource.lenght = 0;
        resource.data = malloc(resource.buffer_size);
        
        if (requested_file != NULL)
        {
            printf("loading %s into memory\n",full_path);
            loadfile(requested_file,&resource);
            status_code = 200;
        }
        else 
        {
            // Will just use a generic 400 for every error for now
            fprintf(stderr,"File %s does not exist\n",full_path);
            snprintf(resource.data,resource.buffer_size,"<h1>400: Bad request<h1>");
            content_type = "text/html";
            resource.lenght = strlen(resource.data);
            status_code = 400;
        }
        
        size_t response_size = 256 + resource.lenght;
        char * server_response = malloc(response_size);

        // Status code handling
        char * status_string = NULL;
        switch (status_code)
        {
        case 200:
            status_string = "OK";
            break;
        case 400:
            status_string = "Bad Request";
            break;
        default:
            status_string = "OK";
            break;
        }


        
        snprintf(server_response,
        response_size,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "\r\n",
        status_code,
        status_string,
        content_type,
        resource.lenght
        );

        size_t header_size = strlen(server_response);

        memcpy(server_response + strlen(server_response),resource.data,resource.lenght);

        send(client_fd, server_response, header_size + resource.lenght, 0);
        close(client_fd);

        free(resource.data);
        free(server_response);
    }
}
