/* Operator shall send 3 arguments when running the program - broadcasr ip, port, string to deliever */ 

#include <stdio.h>      /* printf() and fprintf() */
#include <sys/socket.h> /* socket() and bind() */
#include <arpa/inet.h>  /* sockaddr_in */
#include <stdlib.h>     /* atoi() and exit() */
#include <string.h>     /* memset() */
#include <unistd.h>     /* close() */
#include <netinet/in.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int sock;                         
    struct sockaddr_in broadcast_addr;  
    char *broadcastIP;                
    unsigned short broadcastPort;     
    char *sendString;
    int broadcastPermission;          
    unsigned int sendStringLen;       

    /* Test for correct number of parameters */
    if (argc < 4)   
    {
        fprintf(stderr,"Usage:  %s <IP Address> <Port> <Send String>\n", argv[0]);
        exit(1);
    }

    broadcastIP = argv[1];             
    broadcastPort = atoi(argv[2]);    
    sendString = argv[3];

    /* Setting socket to allow broadcast */
    broadcastPermission = 1;

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
          sizeof(broadcastPermission));

    /* Construct local address structure */
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));   /* Zero out structure */
    broadcast_addr.sin_family = AF_INET;                 /* Internet address family */
    broadcast_addr.sin_addr.s_addr = inet_addr(broadcastIP);/* Broadcast IP address */
    broadcast_addr.sin_port = htons(broadcastPort);         /* Broadcast port */

    sendStringLen = strlen(sendString);  /* Find length of sendString */
    while (1) 
    {
         /* Broadcast sendString in datagram to clients every 3 seconds*/
         sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *) 
               &broadcast_addr, sizeof(broadcast_addr));

        printf("'%s' sent to ip %ld and port %d\n", sendString, broadcast_addr.sin_addr, broadcast_addr.sin_port);

        sleep(3);   /* Avoids flooding the network */
    }
    /* NOT REACHED */
}
