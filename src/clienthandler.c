#include "clienthandler.h"


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
        char full_path[PATH_LIMIT + sizeof(FILE_ROOT)];

        int status_code;

        while ((received = recv(client_fd,user_req + total,sizeof(user_req) - total - 1,0)) > 0)
        {
            total += received;
            user_req[total] = '\0';
            if (strstr(user_req,"\r\n\r\n")) break;
        }

        printf("Request : %s\n",user_req);

        sscanf(user_req,"GET %255s ",path);
        snprintf(full_path,sizeof(full_path),FILE_ROOT "%s",path);

        /*Preparing and sending the response*/

        
        FILE * requested_file = fopen(full_path,"rb");
        size_t resource_size = 64;
        int resource_len = 0;
        char * resource_content = malloc(resource_size);
        if (requested_file)
        {
            
            char c; // look at this guy hes winking at me, i gotta wink bakc ;J
            char * temp;
            for (resource_len = 0;(c = fgetc(requested_file)) != EOF;resource_len++)
            {
                resource_content[resource_len] = c;
                if (resource_len + 1 >= resource_size)
                {
                    resource_size *= 2;
                    temp = realloc(resource_content,resource_size);
                    if (!temp)
                    {
                        perror("Memory allocation issue\n");
                        return 1;
                    }
                    resource_content = temp;
                }
            }
            fclose(requested_file);
            status_code = 200;
        }
        else 
        {
            // Will just use a generic 400 for every error for now
            fprintf(stderr,"File %s does not exist\n",full_path);
            snprintf(resource_content,resource_size,"<h1>400: Bad request<h1>");
            resource_len = strlen(resource_content);
            status_code = 400;
        }
        
        size_t response_size = 256 + resource_len;
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

        // Content-type handling
        char * file_extention = lookup_ext(full_path);
        char * content_type = lookup_mime(file_extention); 

        
        snprintf(server_response,
        response_size,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        status_code,
        status_string,
        content_type,
        resource_len,
        resource_content
        );

        send(client_fd, server_response, strlen(server_response), 0);
        close(client_fd);

        // Okay so the issue is that we free memory that was statically set
        // IF we get an error, so a simple fix would be to make the error thing dynamic too
        free(resource_content);
        free(server_response);
    }
}