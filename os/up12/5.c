#include <stdio.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

volatile char flag = 0;

void handler(int sigid)
{
    switch(sigid){
    case SIGUSR1:
        flag = 1;
        break;
    case SIGUSR2:
        flag = 2;
        break;
    case SIGIO:
        flag = 3;
        break;
    case SIGALRM:
        flag = 4;
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    char *filename = argv[1];
    
    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigaddset(&s1, SIGUSR2);
    sigaddset(&s1, SIGIO);
    sigprocmask(SIG_BLOCK, &s1, &s2);
    
    struct sigaction sa = { .sa_handler = handler, .sa_mask = s1, .sa_flags = SA_RESTART };
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGIO, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);
    
    pid_t pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    if(!pid1){
        int counter = 0;
        unsigned char data = 0;
        while(1) {
            while(!flag) {
                sigsuspend(&s2);
            }
            switch(flag){
            case 1:
                if(counter != CHAR_BIT - 1) {
                    data <<= 1;
                }
                break;
            case 2:
                data |= 1u;
                if(counter != CHAR_BIT -1) {
                    data <<= 1;
                }
                break;
            case 3:
                return 0;
            case 4:
                flag = 0;
                continue;
            default:
                break;
            }
            flag = 0;
            counter++;
            if(counter == CHAR_BIT) {
                write(1, &data, sizeof(data));
                counter = 0;
                data = 0;
            }
            kill(0, SIGALRM);
        }
    }
    
    pid_t pid2 = fork();
    if(pid2 == -1){
        return 1;
    }
    if(!pid2){
        int fd = open(filename, O_RDONLY);
        unsigned char a;
        unsigned char mask = 1u << (CHAR_BIT - 1);
        while(read(fd, &a, sizeof(a)) > 0) {
            for(int i = 0; i < CHAR_BIT; i++) {
                fflush(stdout);
                if(a & mask) {
                    kill(pid1, SIGUSR2);
                }else{
                    kill(pid1, SIGUSR1);
                }
                a <<= 1;
                while(!flag) {
                    sigsuspend(&s2);
                }
                flag = 0;
            }
        }
        kill(pid1, SIGIO);
        close(fd);
        return 0;
    }
    
    wait(NULL);
    wait(NULL);
    
    return 0;
}
