#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define STR_SIZE (4)
int main()
{
    char *pipe_name = "/home/dean/Desktop/Dean/git/sp/test/pingpong/pipe/pipe_name";
    int pfd[2];
    char *ping = "ping";
    char  inbuf[STR_SIZE];

    mkfifo(pipe_name, 0666);

    while(1)
    {
        pfd[1] = open(pipe_name, O_WRONLY);
        printf("pro1 write end fd is %d\n", pfd[1]);
        sleep(1);
        write(pfd[1], "PING", STR_SIZE);
        close(pfd[1]);

        pfd[0] = open(pipe_name, O_RDONLY);
        printf("pro1 read end fd is %d\n", pfd[0]);
        read(pfd[0], inbuf, STR_SIZE);
        puts(inbuf);
        close(pfd[0]);
    }
}