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

enum { KEY = 123444 };

volatile sig_atomic_t sigflag = 0;

int work(int procid, int semid, int *ptr)
{
    while(!sigflag) {
        struct sembuf sops[] =
        {
            { .sem_num = procid, .sem_op = -1 }
        };
        int r = semop(semid, sops, sizeof(sops) / sizeof(sops[0]));
        if(r < 0) {
            continue;
        }
        if(r < 0){
            fprintf(stderr, "semop: %s\n", strerror(errno));
            return 1;
        }
        
        printf("%d %d\n", getpid(), (*ptr)++);
        struct sembuf sops2[] =
        {
            { .sem_num = 1 - procid, .sem_op = 1}
        };
        semop(semid, sops2, sizeof(sops2) / sizeof(sops2[0]));
    }
    semctl(semid, 0, IPC_RMID);
}

void sigint_handler(int a)
{
    sigflag = 1;
}

int main(void)
{
    sigaction(SIGINT, &(struct sigaction) { .sa_flags = SA_RESTART, .sa_handler = sigint_handler }, NULL);
    
    int *ptr = mmap(NULL, sizeof(*ptr), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if(ptr == MAP_FAILED) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        return 1;
    }
    
    int semid = semget(KEY, 2, IPC_CREAT | IPC_EXCL | 0600);
    if(semid < 0) {
        fprintf(stderr, "semget: %s\n", strerror(errno));
        return 1;
    }
    
    semctl(semid, 0, SETVAL, 1);
    
    if(!fork()){
        work(0, semid, ptr);
        _exit(0);
    }
    if(!fork()){
        work(1, semid, ptr);
        _exit(0);
    }
    
    while(wait(NULL) > 0) {}
    return 0;
}
