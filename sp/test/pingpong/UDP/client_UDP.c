/*
** talker.c - a datagram "client" demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT 4950     // the port users will be connecting to
#define MAXBUFLEN 100

int main()
{
    int sockfd;
    struct sockaddr_in their_addr; // connector’s address information
    struct hostent *he;
    int numbytes;
    char buffer[MAXBUFLEN];
    char *msg = "Hello from client";
    int len;
    
    // get the host info
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
    	perror("socket creation failed");
		exit(EXIT_FAILURE);
    }

    //filling server address information
    their_addr.sin_family = AF_INET;     // host byte order
    their_addr.sin_port = htons(MYPORT); //short, network byte order
    their_addr.sin_addr.s_addr = INADDR_ANY; // 
    bzero(&(their_addr.sin_zero), 8);   //zero the rest of the struct
    
    if ((numbytes = sendto(sockfd, (const char *)msg, strlen(msg), MSG_CONFIRM,
    (const struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
    {
        perror("recvfrom failed");
        exit(1);
    }
    printf("sent %d bytes to %s\n", numbytes, inet_ntoa(their_addr.sin_addr));
    recvfrom(sockfd, (char *)buffer, MAXBUFLEN, MSG_WAITALL, (struct sockaddr *)&their_addr, &len);
    buffer[MAXBUFLEN] = '\0';
    printf("server: %s\n", buffer);
    
    close(sockfd);
    return 0;
    }