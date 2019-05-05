#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

enum { KEY = 213499788 };

void work(int semid, int id, int *mem) {
    while(1) {
        struct sembuf smm[] = {
            { .sem_num = id, .sem_op = -1 }
        };
        int r = semop(semid, smm, sizeof(smm) / sizeof(smm[0]));
        if(r < 0){
            printf("semop1: %s\n", strerror(errno));
            _exit(0);
        }
        (*mem)++;
        if(*mem > 100){
            _exit(0);
        }
        printf("i: %d mem: %d\n", id, *mem);
        struct sembuf smm2[] = {
            { .sem_num = 1 - id, .sem_op = 1 }
        };
        r = semop(semid, smm2, sizeof(smm2) / sizeof(smm2[0]));
        if(r < 0){
            printf("semop2: %s\n", strerror(errno));
            _exit(0);
        }
    }
}

int main(void)
{
    int *mem = mmap(NULL, sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int semid = semget(KEY, 2, IPC_CREAT | IPC_EXCL | 0600);
    
    semctl(semid, 0, SETVAL, 1);
    
    for(int i = 0; i < 2; i++) {
        if(!fork()){
            work(semid, i, mem);
            _exit(0);
        }
    }
    wait(NULL);
    semctl(semid, 0, IPC_RMID, 0);
    wait(NULL);
    return 0;
}
