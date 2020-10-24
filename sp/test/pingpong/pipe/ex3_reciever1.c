#include <stdio.h> 
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

#define UNIQUE (70)
#define MSG_SIZE (100)
struct message
{
    long type;
    char text[MSG_SIZE];
}message;

int main()
{   
    int queue_id;
    key_t key = ftok("queue_key", UNIQUE);
    queue_id = msgget(key, 0666 | IPC_CREAT);
    
    msgrcv(queue_id, &message, sizeof(message), 1, 0);
    printf("data recieved1 from sender is: %s\n", message.text);

    msgctl(queue_id, IPC_RMID, NULL);

    return (0);
}