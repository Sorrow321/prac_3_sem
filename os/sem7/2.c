#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

enum{ PHILNUM = 5, KEY = 4444444 };

void life(int semid, char *states, int philid)
{
    while(1) {
        states[philid] = 'W';
        struct sembuf sops1[] = 
        {
            { .sem_num = philid, .sem_op = -1},
            { .sem_num = (philid + 1) % PHILS, .sem_op = -1 }
        };
        semop(semid, sops1, sizeof(sops1) / sizeof(sops1[0]));
        
        states[philid] = 'E';
        usleep(10000);
        
        struct sembuf sopd2[] =
        {
            { .sem_num = philid, .sem_op = -1},
            { .sem_num = (philid + 1) % PHILS, .sem_op = -1 }
        };
        semop(semid, sops2, sizeof(sops1) / sizeof(sops1[0]));
        
        states[philid] = 'T';
        usleep(10000);
    }
}

int main(void)
{
    int *ptr = mmap(NULL, sizeof(*ptr), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if(ptr == MAP_FAILED) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        return 1;
    }
    
    int semid = semget(KEY, PHILNUM, IPC_CREAT | IPC_EXCL | 0600);
    if(semid < 0) {
        fprintf(stderr, "semget: %s\n", strerror(errno));
        return 1;
    }
    
    
    for(int i = 0; i < PHILNUM; i++){
        semctl(semid, i, SETVAL, 1);
    }
    
    for(int i = 0; i < PHILNUM; i++){
        if(!fork()){
            life(semid, ptr, i);
            _exit(0);
        }
    }
    
    while(1) {
        printf("%s\r", ptr);
        usleep(50000);
    }
    
}
