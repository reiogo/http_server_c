#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Create an error output.
void error (const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    // Make sure that the program is run with 2 variables.
    if(argc < 3)
    {
        fprintf(stderr, "Usage %s hostname port\n", argv[0]);
        exit(1);
    }
    //Initialize some variables.
    int sockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr;
    struct hostent *server;

    // Convert portno.
    portno = atoi(argv[2]);
    // Create a socket.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("Error opening socket.");
    }
    server = gethostbyname(argv[1]);
    if(server == NULL)
    {
        fprintf(stderr, "Error, no such host");
    }

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Connection Failed");
    }

    while(1)
    {
        memset(buffer, 0, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if(n < 0)
        {
            error("Error on writing");
        }
        memset(buffer, 0, 255);
        n = read(sockfd, buffer, 255);
        if(n < 0)
        {
            error("Error on reading.");
        }
        printf("Server: %s", buffer);

        int i = strncmp("Bye", buffer, 3);
        if (i == 0)
        {
            break;
        }

    }

    close(sockfd);
    return 0;


}
