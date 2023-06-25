#include "main.h"

#define Psem(a, b); if (semaphore_operation(a, b, 1) == 1){exit(1);}
#define Vsem(a, b); if (semaphore_operation(a, b, -1) == -1){exit(1);}
#define MError(a); perror(a);exit(1);

double execTime(const char* filepath){
    clock_t begin, end;

    begin = clock();
    pid_t pid = fork();
    if(pid == -1){MError("fork");}
    else if(pid == 0){ //C hild
        if(execl(filepath, filepath, NULL) == -1){MError("execl");}
    }
    else{ // Parent
        wait(NULL);
        end = clock();
    }

    return end-begin;
}

void runner(int msgid, int semid, int mytype){
    struct my_message received_message;

    // printf("Child process %d with PID %d\n", mytype, getpid());
    while(1){
        Vsem(semid, mytype-1);
        // Receive a message from the message queue
        if (msgrcv(msgid, &received_message, sizeof(received_message.mtext), mytype, 0) == -1) {MError("msgrcv");}
        // else
        // {
        //     printf("Child %d Received message: %s\n", mytype, received_message.mtext);
        // }

        if(strcmp(received_message.mtext, "stop") == 0) break;
        else{
            double time = execTime(received_message.mtext) / CLOCKS_PER_SEC;
            printf("Tempo = %.2fs\n", time);
        }
    }
}