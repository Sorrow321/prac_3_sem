#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

void work(int procnum, int *mem, int semid, int nproc, int maxval)
{
    while(1) {
        struct sembuf ops[] = {
            { .sem_num = procnum, .sem_op = -1 }
        };
        if(semop(semid, ops, sizeof(ops) / sizeof(ops[0])) < 0){
            _exit(0);
        }
        
        int sender = mem[0];
        printf("%d %d %d\n", procnum + 1, mem[1], sender + 1);
        mem[0] = procnum;
        fflush(stdout);
        mem[1] += 1;
        if(mem[1] > maxval){
            _exit(0);
        }
        int val = mem[1] % nproc;
        unsigned long long res = (val * val) % nproc;
        unsigned long long res2 = (val * val) % nproc;
        res = (res * res2) % nproc;
        struct sembuf ops2[] = {
            { .sem_num = (unsigned short)res, .sem_op = 1 }
        };
        if(semop(semid, ops2, sizeof(ops2) / sizeof(ops2[0])) < 0){
            _exit(0);
        }
    }
}

int main(int argc, char *argv[])
{
    int nproc, maxval;
    key_t key;
    sscanf(argv[1], "%d", &nproc);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &maxval);
    
    int *mem = mmap(NULL, 2 * sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int semid = semget(key, nproc + 1, IPC_CREAT | IPC_EXCL | 0600);
    semctl(semid, 0, SETVAL, 1);
    mem[0] = -1;
    for(int i = 0; i < nproc; i++){
        pid_t pid = fork();
        if(pid == -1){
            return 1;
        }
        if(!pid) {
            work(i, mem, semid, nproc, maxval);
        }
    }
    
    wait(NULL);
    semctl(semid, 0, IPC_RMID);
    for(int i = 1; i < nproc; i++){
        wait(NULL);
    }
    return 0;
}
