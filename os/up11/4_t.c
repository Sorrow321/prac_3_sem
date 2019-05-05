#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

volatile char flag = 0;

void sighandler(int sigid)
{
    flag = 1;
}

int main(int argc, char *argv[])
{
    int N;
    sscanf(argv[1], "%d", &N);
    if(N == 1) {
        printf("Done\n");
        fflush(stdout);
        return 0;
    }
    
    sigset_t ss1, ss2;
    sigemptyset(&ss1);
    sigaddset(&ss1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &ss1, &ss2);
    
    struct sigaction saa = { .sa_handler = sighandler, .sa_flags = SA_RESTART };
    sigaction(SIGUSR1, &saa, NULL);
    
    int fd[2];
    if(pipe(fd) == -1){
        return 1;
    }
    
    pid_t pid1 = -1, pid2 = -1;
    for(int i = 0; i < 2; i++) {
        pid_t pid = fork();
        if(pid == -1){
            return 1;
        }
        
        if(i == 0) {
            pid1 = pid;
        }else{
            pid2 = pid;
        }
        if(!pid){
            FILE *pwr = fdopen(fd[1], "w");
            FILE *prd = fdopen(fd[0], "r");
            setbuf(pwr, NULL);
            setbuf(prd, NULL);
            
            int a = 1;
            pid_t mypid = getpid();
            while(1){
                while(!flag){
                    sigsuspend(&ss2);
                }
                flag = 0;
                pid_t brother;
                if(fscanf(prd, "%d %d", &brother, &a) != 2) {
                    fclose(prd);
                    fclose(pwr);
                    break;
                }
                
                if(a < N) {
                    printf("%d %d\n", i + 1, a);
                    fflush(stdout);
                }
                a++;
                
                if(a >= N) {
                    fclose(prd);
                    fclose(pwr);
                    break;
                }
                
                fprintf(pwr, "%d %d ", mypid, a);
                kill(brother, SIGUSR1);
            }
            return 0;
        }
    }
    
    FILE *pwr = fdopen(fd[1], "w");
    setbuf(pwr, NULL);
    
    fprintf(pwr, "%d 1 ", pid2);
    close(fd[0]);
    
    kill(pid1, SIGUSR1);
    pid_t expid = wait(NULL);
    if(expid == pid1){
        fprintf(pwr, "0 %d ", N);
        kill(pid2, SIGUSR1);
    }else{
        fprintf(pwr, "0 %d ", N);
        kill(pid1, SIGUSR1);
    }
    fclose(pwr);
    wait(NULL);
    
    printf("Done\n");
    fflush(stdout);
    return 0;
}
