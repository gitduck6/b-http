#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ioheader.h"
#include "clienthandler.h"
#include "confhandler.h"


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
    * i know nothing about multi-threading so i need to learn it
    * ill be using posix pthreads because the server only works on unix (windows support coming soon)
    * after making a semi-firm foundation for this program, i plan to look for contributors    
    * 
    * Im cleaning everything up this time instead of adding to/fixing the terrible code
    * 
    * 
\*/

Conf parameters = {0};

int main(void)
{
    /*Setting up the parameters*/
    config_init(&parameters);

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
    addr.sin_port = htons(parameters.port);

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

    printf("Listening on http://localhost:%d\n",parameters.port);
    
    handle_client(server_fd);

    close(server_fd);
    return 0;

}