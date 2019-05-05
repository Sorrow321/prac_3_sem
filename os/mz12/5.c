#include <signal.h>

volatile flag = 0;
volatile flag2 = 0;
volatile unsigned char res = 0;
pid_t son1, son2;

void handl(int sigid)
{
    if(getpid() == son1){
        if(sigid == SIGUSR1) {
            res |= 1;
            res <<= 1;
        }else{
            res <<= 1;
        }
    }else{
        
    }
}

int main(int argc, char *argv[])
{
    char *filename = argv[1];
    
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    struct sigaction sa = { .sa_handler = handl, .sa_flags = SA_RESTART };
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGIO, &sa, NULL);
    
    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigaddset(&s1, SIGUSR2);
    sigaddset(&s1, SIGALRM);
    sigaddset(&s1, SIGIO);
    sigprocmask(SIG_BLOCK, &s1, &s2);
    
    son1 = fork();
    if(son1 == -1){
        return 1;
    }
    if(!son1){
        while(1){
            while(!flag2){
                sigsuspend(&2);
            }
            flag2 = 0;
    }
    
    son2 = fork();
    if(son2 == -1){
        return 1;
    }
    
    if(!son2){
        int fd = open(filename, O_RDONLY);
        unsigned char a;
        while(read(fd, &a, sizeof(a)) > 0) {
            for(int i = 0; i < 8; i++) {
                if(a & 1) {
                    kill(son1, SIGUSR2);
                }else{
                    kill(son1, SIGUSR1);
                }
            }
            while(!flag){
                sigsuspend(&s2);
            }
            flag = 0;
        }else{
            kill(son1, SIGIO);
        }
        close(fd);
        _exit(0);
    }
