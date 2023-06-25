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
//