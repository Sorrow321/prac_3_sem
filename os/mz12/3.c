#include <signal.h>
#include <sys/types.h>
#include <unistd.h> 
#include <stdio.h>

volatile int count = 0;
volatile int flag = 0;

void handl(int sigid)
{
    if(sigid == SIGUSR1) {
        count += 5;
    }else{
        count -= 4;
    }
    printf("%d %d\n", sigid, count);
    if(count < 0){
        _exit(0);
    }
    flag = 1;
}

int main(void)
{
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    
    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigaddset(&s1, SIGUSR2);
    sigprocmask(SIG_BLOCK, &s1, &s2);
    
    struct sigaction sa = { .sa_handler = handl, .sa_flags = SA_RESTART, .sa_mask = s1};
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    
    printf("%d\n", getpid());
    
    while(1) {
        while(!flag) {
            sigsuspend(&s2);
        }
        flag = 0;
    }
    return 0;
}
