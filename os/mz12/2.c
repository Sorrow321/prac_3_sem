#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    unsigned count;
    int modulo;
    char *prog;
    sscanf(argv[1], "%u", &count);
    sscanf(argv[2], "%d", &modulo);
    prog = argv[3];
    
    int fd[2];
    int fild[2];
    if(pipe(fd) == -1 || pipe(fild) == -1){
        return 1;
    }
    pid_t p1 = fork();
    if(p1 == -1){
        return 1;
    }
    
    if(!p1) {
        pid_t p2 = fork();
        if(p2 == -1){
            return 1;
        }
        close(fild[0]);
        if(p2 > 0) {
            // P1
            close(fild[1]);
            close(fd[0]);
            if(dup2(fd[1], 1) == -1){
                return 1;
            }
            close(fd[1]);
            setbuf(stdout, NULL);
            
            for(unsigned i = 1; i <= count; i++) {
                printf("%u ", (i * i) % modulo);
            }
            
            exit(0);
        }else{
            // P2
            close(fd[1]);
            if(dup2(fd[0], 0) == -1 || dup2(fild[1], 1) == -1){
                return 1;
            }
            close(fild[1]);
            close(fd[0]);
            setbuf(stdout, NULL);
            
            execlp(prog, prog, NULL);
            _exit(1);
        }
    }
    close(fd[0]);
    close(fd[1]);
    close(fild[1]);
    
    pid_t p3 = fork();
    if(p3 == -1){
        return 1;
    }
    
    if(!p3) {
        pid_t p4 = fork();
        if(p4 == -1){
            return 1;
        }
        if(p4 > 0) {
            // P3
            close(fild[0]);
            wait(NULL);
            exit(0);
        }else{
            // P4
            if(dup2(fild[0], 0) == -1){
                return 1;
            }
            close(fild[0]);
            setbuf(stdin, NULL);
            
            unsigned a;
            while(scanf("%u", &a) == 1) {
                printf("%u\n", a);
                fflush(stdout);
            }
            exit(0);
        }
    }
    close(fild[0]);
    
    wait(NULL);
    wait(NULL);
    printf("0\n");
    fflush(stdout);
    return 0;
}
