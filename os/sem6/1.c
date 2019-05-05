#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

volatile int flag;
volatile int num;

enum
{
    MAX_NUM = 5
};

void hand(int sid_id)
{
    printf("%d\n", num);
    fflush(stdout);
    num++;
    flag = 1;
}

int main(void)
{
    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGHUP);
    sigprocmask(SIG_BLOCK, &s1, &s2);
    
    struct sigaction sa = { .sa_handler = hand, .sa_flags = SA_RESTART };
    sigaction(SIGHUP, &sa, NULL);
    
    printf("%d\n", getpid());
    fflush(stdout);
    
    while(1) {
        while(!flag) {
            sigsuspend(&s2);
        }
        flag = 0;
        if(num == MAX_NUM) {
            break;
        }
    }
    
    return 0;
}
