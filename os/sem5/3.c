#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t flag;

void handler(int s)
{
    flag = 1;
}

int main()
{
    struct sigaction sa = { .sa_flags = SA_RESTART, .sa_handler = handler };
    sigaction(SIGINT, &sa, NULL);
    sigset_t bs, os;
    
    sigemptyset(&bs);
    sigaddset(&bs, SIGINT);
    sigprocmask(SIG_BLOCK, &bs, &os);
    
    int a,b;
    while(1){
        // BUSY WAIT - очень плохо
        /*
        while(!flag){
        }
        */
        
        while(!flag) {
            // ... Race condition
            sigprocmask(SIG_SETMASK, &os, NULL); // и тут тоже
            pause();
            sigprocmask(SIG_BLOCK, &bs, &os);
            // Ещё один race condition
        }
        
        flag = 0;
        printf("singint\n");
    }
    return 0;
}
