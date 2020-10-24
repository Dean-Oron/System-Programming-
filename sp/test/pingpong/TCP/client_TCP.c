/*
** client.c - a stream socket client demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 3490 // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once

int main()
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr; // connector’s address information
    char *msg = "hello from client";
    // get the host info
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket failed");
        exit(1);
    }
    their_addr.sin_family = AF_INET;    // host byte order
    their_addr.sin_port = htons(PORT); // short, network byte order
    
    if (inet_pton(AF_INET, "127.0.0.1", &their_addr.sin_addr) <=0)
    {
        perror("inet_pton failed: Invalid address\n");
        exit(1);
    }
   
    bzero(&(their_addr.sin_zero), 8);   // zero the rest of the struct
  
    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) 
    {
        perror("connect failed");
        exit(1);
    }

    send(sockfd, msg, strlen(msg), 0);
    printf("msg send");
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) 
    {
        perror("recv failed");
        exit(1);
    }
    buf[numbytes] = '\0';
    printf("Received: %s",buf);
    close(sockfd);
    return (0);
}