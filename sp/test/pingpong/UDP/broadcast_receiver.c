#include <stdio.h>      /* printf() and fprintf() */
#include <sys/socket.h> /* socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* sockaddr_in and inet_addr() */
#include <stdlib.h>     /* atoi() and exit() */
#include <string.h>     /* memset() */
#include <unistd.h>     /* close() */

#define MAXRECVSTRING 255  /* largest size of string allowed */

int main(int argc, char *argv[])
{
    int sock;                         
    struct sockaddr_in broadcastAddr; 
    unsigned short broadcastPort;     
    char recvString[MAXRECVSTRING + 1]; 
    int recvStringLen;

    /* will hold the port of sender (broadcast port ) */
    broadcastPort = atoi(argv[1]);   

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* Initilizing broadcast address */ 
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));   
    broadcastAddr.sin_family = AF_INET;               
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    broadcastAddr.sin_port = htons(broadcastPort);
  
    bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr));

    while(1)
    {
        recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, NULL, 0);
        recvString[recvStringLen] = '\0';
        printf("'%s' Received from broadcast ip %ld and port %d\n", recvString, broadcastAddr.sin_addr, broadcastAddr.sin_port);
    }

    close(sock);
    exit(0);
}
