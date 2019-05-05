#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>

enum { KEY = 22148199 };

int absol(int a){
    if(a >= 0){
        return a;
    }
    return -a;
}

int main(int argc, char *argv[])
{
    int count;
    sscanf(argv[1], "%d", &count);
    
    int sem_id = semget(KEY, count, IPC_CREAT | IPC_EXCL | 0600);
    if(sem_id < 0){
        return 1;
    }
    
    for(int i = 0; i < count; i++){
        pid_t pid = fork();
        if(pid == -1){
            return 1;
        }
        if(!pid) {
            setbuf(stdin, NULL);
            setbuf(stdout, NULL);
            while(1){
                struct sembuf down[] = {
                    { .sem_num = i, .sem_op = -1 }
                };
                if(semop(sem_id, down, sizeof(down) / sizeof(down[0])) < 0){
                    return 0;
                }
                
                int a;
                if(scanf("%d", &a) != 1){
                    return 0;
                }
                printf("%d %d\n", i, a);
                
                struct sembuf up[] = {
                    { .sem_num = (a % count + count) % count, .sem_op = 1 }
                };
                if(semop(sem_id, up, sizeof(up) / sizeof(up[0])) < 0){
                    return 0;
                }
            }
        }
    }
    semctl(sem_id, 0, SETVAL, 1);
    wait(NULL);
    semctl(sem_id, 0, IPC_RMID);
    for(int i = 1; i < count; i++){
        wait(NULL);
    }
    return 0;
}
