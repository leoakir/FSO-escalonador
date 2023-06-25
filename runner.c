#include "main.h"

int runner(int msgid, int semid, int mytype){
    struct my_message received_message;

    printf("Child process %d with PID %d\n", mytype, getpid());
    if (semaphore_operation(semid, mytype - 1, -1) == -1)
    {
        exit(1);
    }
    // Receive a message from the message queue
    if (msgrcv(msgid, &received_message, sizeof(received_message.mtext), mytype, 0) == -1)
    {
        perror("msgrcv");
        exit(1);
    }
    else
    {
        printf("Child %d Received message: %s\n", mytype, received_message.mtext);
    }
}