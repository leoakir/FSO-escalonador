#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/msg.h>

#define MAX_MSG_SIZE 100
#define N_AUX 4

struct my_message
{
    long mtype;
    char mtext[MAX_MSG_SIZE];
};

// Union for semaphore operations
union semun
{
    int val;               // Value for SETVAL
    struct semid_ds *buf;  // Buffer for IPC_STAT, IPC_SET
    unsigned short *array; // Array for GETALL, SETALL
};

key_t getKey();
int escalonador(int process_count, int process_types[process_count]);
int semaphore_operation(int semid, int semnum, int op);
void runner(int msgid, int semid, int mytype);
int getMsgid(key_t key);
int getSemid(key_t key, int num_sem);
void initSem(int semid, union semun arg, int num_sem);
