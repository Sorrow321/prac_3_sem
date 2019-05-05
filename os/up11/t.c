#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

volatile char father_flag = 0;

void father_handler(int sigid)
{
    printf("signal handled\n");
    fflush(stdout);
    father_flag++;
}

int main(int argc, char *argv[])
{
    
    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &s1, &s2);
    
    struct sigaction sa = { .sa_handler = father_handler, .sa_flags = SA_RESTART  };
    sigaction(SIGUSR1, &sa, NULL);

    pid_t pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    if(!pid1){
        
        printf("kill ret %d\n", kill(getppid(), SIGUSR1));
        fflush(stdout);
        return 0;
    }
    
    printf("Kek\n");
    fflush(stdout);
    while(father_flag != 1){
        sigsuspend(&s2);
    }
    
    struct sigaction sa = { .sa_handler = father_handler, .sa_flags = SA_RESTART  };
    sigaction(SIGUSR1, &sa, NULL);

    pid_t pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    if(!pid1){
        
        printf("kill ret %d\n", kill(getppid(), SIGUSR1));
        fflush(stdout);
        return 0;
    }
    
    printf("Kek\n");
    fflush(stdout);
    while(father_flag != 1){
        sigsuspend(&s2);
    }
    
    
    return 0;
}
