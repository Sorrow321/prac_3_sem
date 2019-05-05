#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int count, modulo;
    char *prog = argv[3];
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &modulo);
    
    int fd24[2];
    if(pipe(fd24) == -1) {
        return 1;
    }
    
    pid_t pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    if(!pid1){
        // P1
        close(fd24[0]);
        int fd12[2];
        if(pipe(fd12) == -1){
            return 1;
        }
        
        pid_t pid2 = fork();
        if(pid2 == -1){
            return 1;
        }
        if(!pid2){
            // P2
            close(fd12[1]);
            if(dup2(fd12[0], 0) == -1 || dup2(fd24[1], 1) == -1) {
                return 1;
            }
            close(fd12[0]);
            close(fd24[1]);
            
            //setbuf(stdin, NULL);
            //setbuf(stdout, NULL);
            
            execlp(prog, prog, NULL);
            _exit(1);
            
        }
        close(fd12[0]);
        if(dup2(fd12[1], 1) == -1) {
            return 1;
        }
        close(fd12[1]);
        close(fd24[1]);
        
        //setbuf(stdin, NULL);
        //setbuf(stdout, NULL);
        
        for(int i = 1; i <= count; i++) {
            unsigned long long temp = i;
            temp %= modulo;
            temp *= temp;
            temp %= modulo;
            printf("%u ", (unsigned)temp);
            fflush(stdout);
        }
        close(1);
        wait(NULL);
        return 0;
    }
    close(fd24[1]);
    
    pid_t pid3 = fork();
    if(pid3 == -1){
        return 1;
    }
    if(!pid3){
        // P3
        
        pid_t pid4 = fork();
        if(pid4 == -1){
            return 1;
        }
        if(!pid4) {
            // P4
            if(dup2(fd24[0], 0) == -1) {
                return 1;
            }
            close(fd24[0]);
            
            //setbuf(stdin, NULL);
            //setbuf(stdout, NULL);
            
            int a;
            while(scanf("%d", &a) == 1) {
                printf("%d\n", a);
                fflush(stdout);
            }
            return 0;
        }
        close(fd24[0]);
        wait(NULL);
        return 0;
    }
    close(fd24[0]);
    
    wait(NULL);
    wait(NULL);
    printf("0\n");
    return 0;
}
