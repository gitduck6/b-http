#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PATH_LIMIT 256
#define FILE_ROOT "www"

#define STATUS_OK 200
#define BAD_REQUEST 400

/*\
    * REMAKING IT COS I SUCK
    * I think i know this stuff better now and will try to use functions such as scanf and snprintf better
    * 
    * Also its kinda confusing to write this code on windows,
    * my testing enviroment is just msys2
    * But VScode pretty much doesnt tell me the errors
    * only that syssocket and netinet cant be opened lol
    * do i maybe switch to linux
    * 
    * Also why do i only now notice that i dont keep the tcp sessions
    * rather make one for every request
    * 
    * BRO coding in python made me forgot putting semi-colons at the end lmao
    * also im a bit worried about binary files in the future
    * which arent guaranteed to be perfectly null terminated
    * and that can cause strlen to malfunction
    * 
    * Chat im lowk addicted to this (last words befoer quiting coding for 6 months)
    * 
\*/

int main(void)
{
    /*Creating a socket*/
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        fprintf(stderr,"Creating a socket failed.\n");
        return 1;
    }

    /*Binding it to a port*/
    struct sockaddr_in addr = {0};
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&addr,sizeof(addr)) < 0)
    {
        fprintf(stderr,"Could not bind port.\n");
        return 1;
    }

    /*Start listening to the port*/
    if (listen(server_fd,10) < 0)
    {
        fprintf(stderr,"Listen failed.\n");
        return 1;
    }

    printf("Listening on http://localhost:8080\n");

    char running = 1;
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
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        status_code,
        status_string,
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
    close(server_fd);
    return 0;

}