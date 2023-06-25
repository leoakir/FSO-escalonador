#include "main.h"

int escalonador(int N_AUX){
    //
    key_t key;
    int msgid;
    int semid[N_AUX];
    union semun arg;
    struct my_message message, received_message;

    // Create a unique key for the message queue
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    if ((key = ftok(cwd, 'A')) == -1)
    {
        perror("ftok");
        exit(1);
    }

    // Create a new message queue or get the ID of an existing one
    if ((msgid = msgget(key, IPC_CREAT | 0666)) == -1)
    {
        perror("msgget");
        exit(1);
    }

    // Create a new semaphore or get the ID of an existing one
    for (int i = 0; i < N_AUX; i++)
    {
        if ((semid[i] = semget(key, 1, IPC_CREAT | 0666)) == -1)
        {
            perror("semget");
            exit(1);
        }
    }

    // Initialize the semaphore with a value of 1
    arg.val = 0;
    for(int i = 0; i < N_AUX; i++){
        if (semctl(semid[i], 0, SETVAL, arg) == -1)
        {
            perror("semctl");
            exit(1);
        }
    }
    //
    pid_t pidVector[N_AUX];

    for (int i = 0; i < N_AUX; i++)
    {
        pid_t pid = fork();
        pidVector[i] = pid;
        int mytype = i+1;
        // if (pid != 0 && pid != -1){
        //     message.mtype = mytype;                      // Set the message type
            
        //     strcpy(message.mtext, cwd); // Set the message data

        //     if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1)
        //     {
        //         perror("msgsnd");
        //         exit(1);
        //     }
        //     printf("Parent making message\n");
        //     if (semaphore_operation(semid[i], 0, 1) == -1)
        //     {
        //         exit(1);
        //     }
        // }
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        else if (pid == 0)
        {
            //runner();
            // Child process
            printf("Child process %d with PID %d\n", i + 1, getpid());
            if (semaphore_operation(semid[mytype-1], 0, -1) == -1)
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
            return 0; // Child process exits here
        }
    }

    // Parent process
    printf("Parent process with PID %d\n", getpid());
    // for(int i = 0; i < N_AUX; i++){
    //     printf("My Child: %d\n", pidVector[i]);
    // }
    for (int i = 0; i < N_AUX; i++)
    {
        message.mtype = i+1; // Set the message type

        strcpy(message.mtext, cwd); // Set the message data

        if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
        printf("Parent making message\n");
        if (semaphore_operation(semid[i], 0, 1) == -1)
        {
            exit(1);
        }
    }

        
    // Wait for all child processes to finish
    for (int i = 0; i < N_AUX; i++)
    {
        wait(NULL);
    }
    //
    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }
    //
}