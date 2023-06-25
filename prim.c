#include "main.h"

// Function to perform semaphore operations
int semaphore_operation(int semid, int semnum, int op)
{
    struct sembuf sb;
    sb.sem_num = semnum;   // Semaphore index in the array
    sb.sem_op = op;        // Semaphore operation (positive or negative value)
    sb.sem_flg = SEM_UNDO; // Enable undo

    if (semop(semid, &sb, 1) == -1)
    {
        perror("semop");
        return -1; // Error occurred
    }

    return 0; // Successful operation
}

key_t getKey(){
    key_t key;
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    if ((key = ftok(cwd, 'A')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    return key;
}

int getMsgid(key_t key){
    int msgid;
    if ((msgid = msgget(key, IPC_CREAT | 0666)) == -1)
    {
        perror("msgget");
        exit(1);
    }
    return msgid;
}

int getSemid(key_t key, int num_sem){
    int semid;
    if ((semid = semget(key, num_sem, IPC_CREAT | 0666)) == -1)
    {
        perror("semget");
        exit(1);
    }
    return semid;
}

void initSem(int semid , union semun arg, int num_sem){
    unsigned short sem_initial_values[num_sem];
    for (int i = 0; i < num_sem; i++)
    {
        sem_initial_values[i] = 0;
    }
    arg.array = 0;
    arg.array = sem_initial_values;
    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("semctl");
        exit(1);
    }
}