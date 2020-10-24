/*
** listener.c - a datagram sockets "server" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> //perror()
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT 4950 // the port users will be connecting to
#define MAXBUFLEN 100

int main(void)
{
	int sockfd; 
	struct sockaddr_in my_addr; // my address information
	struct sockaddr_in their_addr; // connector’s address information sendto();
	int addr_len, numbytes;
	char buf[MAXBUFLEN];
	char *msg = "Hello from server";

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	//filing server address information
	my_addr.sin_family = AF_INET;	//host byte order. IPv4
	my_addr.sin_port = htons(MYPORT); //short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; //automatically fill with my IP
	//zero the rest of the struct
	bzero(&(my_addr.sin_zero), 8);

	if (bind(sockfd, (const struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) 
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	addr_len = sizeof(struct sockaddr);
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
	{
		perror("recvfrom failed");
		exit(EXIT_FAILURE);
	}

	printf("got packet from(without ntoa();) %d\n",their_addr.sin_addr);	
	printf("got packet from %s\n",inet_ntoa(their_addr.sin_addr));
	printf("packet is %d bytes long\n",numbytes);
	buf[numbytes] = '\0';
	
	sendto(sockfd, msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *)&their_addr, sizeof(their_addr));
	printf("packet contains \"%s\"\n",buf);
	close(sockfd);
	return (0);
}
