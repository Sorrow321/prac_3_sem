#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

volatile char flag;
volatile char father_flag;

void sighandler(int sigid)
{
    flag = 1;
}

void father_handler(int sigid)
{
    father_flag++;
}

int main(int argc, char *argv[])
{
    int N;
    sscanf(argv[1], "%d", &N);
    
    sigset_t ss1, ss2;
    sigemptyset(&ss1);
    sigaddset(&ss1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &ss1, &ss2);
    
    struct sigaction saa = { .sa_handler = father_handler, .sa_flags = SA_RESTART };
    sigaction(SIGUSR1, &saa, NULL);
    
    pid_t pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    
    if(!pid1){
        struct sigaction sa = { .sa_handler = sighandler, .sa_flags = SA_RESTART };
        sigaction(SIGUSR1, &sa, NULL);
        
        sigset_t s1, s2;
        sigemptyset(&s1);
        sigaddset(&s1, SIGUSR1);
        sigprocmask(SIG_BLOCK, &s1, &s2);
        
        kill(getppid(), SIGUSR1);
        
        int a = 1;
        pid_t mypid = getpid();
        while(1){
            while(!flag){
                sigsuspend(&ss2);
            }
            printf("im here1");
            fflush(stdout);
            flag = 0;
            return 0;
        }
        
        return 0;
    }
    
    while(father_flag != 1) { // оба сына готовы принимать сигналы
        sigsuspend(&ss2);
    }
    
    printf("son1: %d\n", pid1);
    fflush(stdout);
    kill(pid1, SIGUSR1);
    wait(NULL);
    
    printf("Done\n");
    fflush(stdout);
    return 0;
}
