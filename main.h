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

int escalonador();
int semaphore_operation(int semid, int semnum, int op);