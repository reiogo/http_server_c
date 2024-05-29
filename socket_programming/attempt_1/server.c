#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Create an error output.
void error (const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    // Make sure that the program is run with 2 variables.
    if(argc < 2)
    {
        fprintf(stderr, "Port No. not provided. Program terminated\n");
        exit(1);
    }
    //Initialize some variables.
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    // Create a socket.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd <0)
    {
        error("Error opening socket.");
    }
    // Clear &serv_addr.
    memset ((char *) &serv_addr, 0, sizeof(serv_addr));
    // Convert the second argument to an integer.
    portno = atoi(argv[1]);
    // Fill out some of the struct serv_addr struct.
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    // Bind to the socket.
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
    {
        error("Binding Failed.");
    }

    // Listen on the newly bound socket.
    listen(sockfd, 5);
    
    // what does this do?
    clilen = sizeof(cli_addr);

    // Accept the incoming request.
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    // If it doesn't work then error.
    if(newsockfd < 0)
    {
        error("Error on Accept");
    }
    while(1)
    {
        memset(buffer, 0, 255);
        n = read(newsockfd, buffer, 255);
        if(n < 0)
        { 
            error("Error on reading.");
        }
        printf("Client: %s\n", buffer);
        memset(buffer, 0, 255);
        fgets(buffer, 255, stdin);

        n = write(newsockfd, buffer,strlen(buffer));
        if (n < 0)
        {
            error("Error on Writing.");
        }

        int i = strncmp("Bye", buffer, 3);
        if(i == 0)
        {
            break;
        }

    }
    
    close(newsockfd);
    close(sockfd);
    return 0;

}
