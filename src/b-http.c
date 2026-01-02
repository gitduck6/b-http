#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

    char running = 1;
    while (running)
    {
        int client_fd = accept(server_fd,NULL,NULL);

        if (client_fd < 0)
        {
            perror("Accept error\n");
            continue;
        }

        char user_req[4096];
        int recieved;
        int total = 0;

        while ((recieved = recv(client_fd, user_req + total, sizeof(user_req) - total - 1, 0)) > 0)
        {
            total += recieved;
            if (total >= (sizeof(user_req) - 1)) break;
        }
        if (recieved < 0)
        {
            perror("Recv failed");
            close(client_fd);
            continue;
        }

        user_req[total] = '\0';

        char * server_content = "Welcome home\n";
        char server_response[512];

        snprintf(server_response,
        sizeof(server_response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        strlen(server_content),
        server_content
        );

        send(client_fd, server_response, strlen(server_response), 0);
        close(client_fd);
    }
    close(server_fd);
    return 0;

}