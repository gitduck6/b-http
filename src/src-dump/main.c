#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "filehandler.h"

#define METHOD_GET "GET"
#define FILES_DIR "www"

/*\
    * This is a simple C http server that im trying to improve in order to learn
    * right now all it does is
    * Create a socket, Bind that socket to a port, listen in that port, and respond to clients
    * 
    * rightnow it needs many improvements, 
    * such as responding from a file,
    * handling multiple clients at once,
    * giving the file requested by the client (images, css, js included)
    * 
    * 
\*/

int main(void)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        fprintf(stderr,"Creating a socket failed.\n");
        return 1;
    }

    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&addr,sizeof(addr)) < 0)
    {
        fprintf(stderr,"Could not bind port.\n");
        return 1;
    }

    if (listen(server_fd,10) < 0)
    {
        fprintf(stderr,"Listen failed.\n");
        return 1;
    }

    printf("Listening on http://localhost:8080\n");

    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0)
        {
            fprintf(stderr,"Accept error.\n");
            continue;
        }

        string buffer;
        buffer.size = 4096;
        buffer.content = malloc(buffer.size);
        int recieved;
        int total = 0;

        while ((recieved = recv(client_fd, buffer.content + total,buffer.size - total - 1,0)) > 0)
        {
            total += recieved;
            buffer.content[total] = '\0';
            buffer.len = total;
            if (strstr(buffer.content, "\r\n\r\n")) break;
        }

        if (recieved > 0)
        {
            printf("Request:\n%s\n",buffer.content);
        }

        // OKAY even if this works and im happy, this is extremely dangerous,
        // a hacker could potentially read ANY file in the system
        char path[512] = FILES_DIR;
        if (strstr(buffer.content,METHOD_GET) == buffer.content)
        {
            char * startOfPath = buffer.content + sizeof(METHOD_GET);
            for (int i = 0;(startOfPath[i] != ' ') && ((i + 1) < (sizeof(path) - sizeof(FILES_DIR)));i++)
            {
                path[i + sizeof(FILES_DIR) -1 ] = startOfPath[i];
            }
        }



        string body = loadFile(path);
        string response;
        response.len = sizeOfFile(path);

        // ERNNO's to http response codes
        if ()

        response.size = 256 + response.len;
        response.content = malloc(response.size);
        // OMG IM SO STUPID

        printf("PATH IS %s\n",path);

        snprintf(response.content,  response.size,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        strlen(body.content),
        body.content        
        );

        send(client_fd, response.content, strlen(response.content), 0);

        free(response.content);
        free(buffer.content);
        free(body.content);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}