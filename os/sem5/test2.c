#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

volatile int flag;

void hand(int sid_id)
{
    flag = 1;
}

int main(void)
{
    printf("%d\n", getpid());
    fflush(stdout);
    
    unsigned num = 0;
    struct sigaction sa = { .sa_handler = hand };
    sigaction(SIGINT, &sa, NULL);

    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGINT);
    sigprocmask(SIG_BLOCK, &s1, &s2);
    
    while(1) {
        while(!flag) {
            sigsuspend(&s2);
        }
        
        printf("%d\n", num);
        fflush(stdout);
        num++;
        flag = 0;
        if(num == 4) {
            break;
        }
    }
    return 0;
}
