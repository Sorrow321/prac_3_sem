#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

enum
{
    MODESUM = -1,
    MODEMULT = 1
};

volatile int mode = MODESUM;

void hand(int sid_id)
{
    if(sid_id == SIGINT) {
        mode = MODESUM;
    }else{
        mode = MODEMULT;
    }
}

int main(void)
{
    int sum = 0;
    struct sigaction sa = { .sa_handler = hand, .sa_flags = SA_RESTART };
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    
    printf("%d\n", getpid());
    fflush(stdout);
    
    int a;
    while(scanf("%d", &a) == 1) {
        if(mode == MODESUM){
            sum += (long long)a;
            printf("%d\n", sum);
        }else{
            sum *= (long long)a;
            printf("%d\n", sum);
        }
        fflush(stdout);
    }
    return 0;
}
