#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>

//standard HTTP port
#define SERVER_PORT 80

#define MAXLINE 4096
#define SA struct sockaddr

// having a function that runs when an error occurs
void err_n_die(const char *fmt, ...);


int main(int argc, char **argv)
{
    int             sockfd, n; //socket file descriptor
    int             sendbytes;
    struct sockaddr_in6  servaddr6;
    char            sendline[MAXLINE];
    char            recvline[MAXLINE];


    // usage check: see if the file is run with the right variables
    if (argc !=2)
        err_n_die("usage: %s <server address>", argv[0]);


    // create a socket
    if( (sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
        err_n_die("Error while creating the socket!");


    // clear the memory at servaddr
    bzero(&servaddr6, sizeof(servaddr6));
    // specify address family
    servaddr6.sin6_family = AF_INET6;
    // specify name of the server port
    servaddr6.sin6_port   = htons(SERVER_PORT);
    
    
    // str to binary representation of ip address
    if (inet_pton(AF_INET6, argv[1], &servaddr6.sin6_addr) <= 0)
        err_n_die("inet_pton error for %s ", argv[1]);


    //  try to connect to the server
    if (connect(sockfd, (SA *) &servaddr6, sizeof(servaddr6)) < 0)
        err_n_die("connect failed!");

    // here we've connected. now preparing a message to send back
    sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
    sendbytes = strlen(sendline);

    // send the request 
    if (write(sockfd, sendline, sendbytes) != sendbytes)
        err_n_die("write error");


    memset(recvline, 0, MAXLINE);
    // read the server's response
    while ( (n = read(sockfd, recvline, MAXLINE-1)) > 0)
    {
        printf("%s", recvline);
        memset(recvline, 0, MAXLINE);
    }

    if (n < 0)
        err_n_die("read error");

exit(0);

}


void err_n_die(const char *fmt, ...)
{
    int errno_save;
    va_list     ap;
   
    //any system or library call can set errno so save now
    errno_save = errno;

    //print out the fmt+args to stdout
    va_start(ap, fmt);
    vfprintf(stdout,fmt,ap);
    fprintf(stdout,"\n");
    fflush(stdout);

    //print out error message 
    if (errno_save != 0)
    {
        fprintf(stdout, "(errno= %d): %s\n",errno_save,
                strerror(errno_save));
        fprintf(stdout, "\n");
        fflush(stdout);
    }
    va_end(ap);

    // this is the die part. terminate with an error
    exit (1);
}
