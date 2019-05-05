#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/signalfd.h>


volatile sig_atomic_t flag;

void handler(int s)
{
    flag = 1;
}

int main()
{   
    sigset_t bs, os;
    sigemptyset(&bs);
    sigaddset(&bs, SIGINT);
    sigprocmask(SIG_BLOCK, &bs, &os);
    
    int fd = signalfd(-1, &bs, 0);
    
    int a,b;
    while(1){
        
        struct signalfd_siginfo ssi;
        ssize_t z = read(fd, &ssi, sizeof(ssi));
        printf("sig: %zu %d\n", z, ssi.ssi_signo);
    }
    return 0;
}
