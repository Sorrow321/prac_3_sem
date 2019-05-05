#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

volatile int mult = -1;

void hand(int sid_id)
{
    mult *= -1;
}

int main(void)
{
    printf("%d\n", getpid());
    fflush(stdout);
    
    struct sigaction sa = { .sa_handler = hand, .sa_flags =SA_RESTART };
    sigaction(SIGINT, &sa, NULL);
    
    int a;
    while(scanf("%d", &a) == 1) {
        if(mult == -1){
            printf("%d\n", a * -1);
        }else{
            printf("%d\n", a * a);
        }
        fflush(stdout);
    }
    return 0;
}
