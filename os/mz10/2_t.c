#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];
    if(pipe(fd)){
        return 1;
    }
    pid_t pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    if(pid1 > 0) {
        // отец
        int a;
        close(fd[0]);
        while(scanf("%d", &a) == 1){
            if(write(fd[1], &a, sizeof(a)) == -1){
                return 1;
            }
        }
        close(fd[1]);
        wait(NULL);
    }else{
        // сын
        pid_t pid2 = fork();
        if(pid2 == -1){
            return 1;
        }
        
        if(pid2 > 0){
            // сын
            close(fd[1]);
            close(fd[0]);
            wait(NULL);
        }else{
            // внук
            close(fd[1]);
            long long s = 0;
            int a;
            while(read(fd[0], &a, sizeof(a)) > 0){
                s += a;
            }
            printf("%lld\n", s);
            fflush(stdout);
            close(fd[0]);
            return 0;
        }
    }
    return 0;
}
