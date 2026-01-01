#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

int main()
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

        char buffer[4096];
        int recieved;
        int total = 0;

        while ((recieved = recv(client_fd, buffer + total,sizeof(buffer) - total - 1,0)) > 0)
        {
            total += recieved;
            buffer[total] = '\0';
            if (strstr(buffer, "\r\n\r\n")) break;
        }

        if (recieved > 0)
        {
            buffer[recieved] = '\0';
            printf("Request:\n%s\n",buffer);
        }

        const char *body = "<h1>Hello World!</h1>";
        char response[512];

        snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        strlen(body),
        body        
        );

        send(client_fd, response, strlen(response), 0);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}