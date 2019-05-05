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
        FILE *f = fdopen(fd[1], "w");
        setbuf(f, NULL);
        while(scanf("%d", &a) == 1){
            if(!f) {
                return 1;
            }
            fprintf(f, "%d ", a);
        }
        fclose(f);
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
            FILE *f = fdopen(fd[0], "r");
            setbuf(f, NULL);
            while(fscanf(f, "%d", &a) == 1){
                s += a;
            }
            printf("%lld\n", s);
            fflush(stdout);
            fclose(f);
            return 0;
        }
    }
    return 0;
}
