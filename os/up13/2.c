#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>

enum { MOD = 10 };

void operation(int *data, int ind1, int ind2, int value);

int main(int argc, char *argv[])
{
    unsigned count, nproc, iter_count;
    key_t key;
    sscanf(argv[1], "%u", &count);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%u", &nproc);
    sscanf(argv[4], "%u", &iter_count);
    
    int *mem = mmap(NULL, count * sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(mem == MAP_FAILED) {
        return 1;
    }
    for(unsigned i = 0; i < count; i++) {
        scanf("%d", &mem[i]);
    }
    
    int sem_id = semget(key, count, IPC_CREAT | IPC_EXCL | 0600);
    if(sem_id < 0){
        return 1;
    }
    for(unsigned i = 0; i < count; i++){
        semctl(sem_id, i, SETVAL, 1);
    }
    
    for(unsigned i = 0; i < nproc; i++) {
        pid_t pid = fork();
        if(pid == -1){
            return 1;
        }
        if(!pid) {
            unsigned seed;
            sscanf(argv[5 + i], "%u", &seed);
            srand(seed);
            for(unsigned j = 0; j < iter_count; j++) {
                int ind1 = rand() % count; 
                int ind2 = rand() % count;
                int randvalue = rand() % MOD;
                
                struct sembuf down[] =  {
                    { .sem_num = ind1, .sem_op = -1 },
                    { .sem_num = ind2, .sem_op = -1}
                };
                if(ind1 != ind2) {
                    semop(sem_id, down, sizeof(down) / sizeof(down[0]));
                }else{
                    semop(sem_id, down, (sizeof(down) / sizeof(down[0])) / 2);
                }
                operation(mem, ind1, ind2, randvalue);
                struct sembuf up[] =  {
                    { .sem_num = ind1, .sem_op = 1 },
                    { .sem_num = ind2, .sem_op = 1}
                };
                if(ind1 != ind2) {
                    semop(sem_id, up, sizeof(up) / sizeof(up[0]));
                }else{
                    semop(sem_id, up, (sizeof(up) / sizeof(up[0])) / 2);
                }
            }
            
            return 0;
        }
    }
    
    for(unsigned i = 0; i < nproc; i++) {
        wait(NULL);
    }
    for(unsigned i = 0; i < count; i++) {
        printf("%d\n", mem[i]);
    }
    munmap(mem, count * sizeof(*mem));
    semctl(sem_id, 0, IPC_RMID);
    return 0;
}
