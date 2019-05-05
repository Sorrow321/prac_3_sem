#include <sys/signal.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

volatile int last_prime;
volatile int counter;

enum
{
    MAXCOUNTER = 4
};

void sighand(int sigid)
{
    if(sigid == SIGTERM) {
        _exit(0);
    }else{
        counter++;
        if(counter == MAXCOUNTER) {
            _exit(0);
        }
        printf("%d\n", last_prime);
    }
}

int main(void)
{
    setbuf(stdout, NULL);
    struct sigaction sa = { .sa_handler = sighand, .sa_flags = SA_RESTART };
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    
    int low, high;
    scanf("%d %d", &low, &high);
    printf("%d\n", getpid());
    for(int i = low; i < high; i++) {
        char not_prime = 0;
        for(int j = 2; j <= sqrt(i); j++) {
            if(!(i % j )) {
                not_prime = 1;
                break;
            }
        }
        if(!not_prime){
            last_prime = i;
        }
    }
    printf("-1\n");
    return 0;
}
