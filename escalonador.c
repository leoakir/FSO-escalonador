#include "main.h"

char * program_path(int type){
    char* path = (char *) malloc(sizeof(char) * 20);

    if(type == 0) strcpy(path, "programas/rapido");
    else if(type == 1) strcpy(path, "programas/medio");
    else if(type == 2) strcpy(path, "programas/demorado");
    
    return path;
}

int escalonador(int process_count, int process_types[process_count]){
    //
    union semun arg;
    struct my_message message, received_message;

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    key_t key = getKey();
    int msgid, semid;
    msgid = getMsgid(key);
    semid = getSemid(key, N_AUX);

    // Initialize the semaphore with a value of 0
    arg.array = 0;
    if (semctl(semid, 0, SETVAL, arg) == -1){
        perror("semctl");
        exit(1);
    }

    pid_t pidVector[N_AUX];
    for (int i = 0; i < N_AUX; i++)
    {
        pid_t pid = fork();
        pidVector[i] = pid;
        int mytype = i+1;

        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        else if (pid == 0)
        {
            // Child process
            runner(msgid, semid, mytype);

            return 0; // Child process exits here
        }
    }

    // Parent process
    printf("Parent process with PID %d\n", getpid());
    // for(int i = 0; i < N_AUX; i++){printf("My Child: %d\n", pidVector[i]);}

    for (int i = 0; i < process_count; i++)
    {
        message.mtype = (i%N_AUX)+1; // Set the message type

        strcpy(message.mtext, cwd); // Set the message data
        strcat(message.mtext, program_path(process_types[i]));

        if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
        printf("Parent making message %d\n", i+1);
        if (semaphore_operation(semid, i%4, 1) == -1)
        {
            exit(1);
        }
    }

        
    // Wait for all child processes to finish
    for (int i = 0; i < N_AUX; i++)
    {
        wait(NULL);
    }
    
    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("semctl");
        exit(1);
    }
    //
}