#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int n;
    sscanf(argv[1], "%d", &n);
    int fd1[2], fd2[2];
    if(pipe(fd1) || pipe(fd2)){
        return 1;
    }
    
    pid_t pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    
    int num = 1;
    if(pid1 > 0) {
        pid_t pid2 = fork();
        if(pid2 == -1){
            return 1;
        }
        if(pid2 > 0){
            close(fd1[1]);
            close(fd2[0]);
            close(fd1[0]);
            close(fd2[1]);
            wait(NULL);
            wait(NULL);
            printf("Done\n");
        }else{
            // второй сын
            close(fd1[1]);
            close(fd2[0]);
            FILE * rr = fdopen(fd1[0], "r");
            FILE * wr = fdopen(fd2[1], "w");
            while(num < n) {
                fscanf(rr, "%d", &num);
                printf("%d %d\n", num, getpid());
                fflush(stdout);
                num++;
                fprintf(wr, "%d", num);
                fflush(wr);
                if(num >= n){
                    break;
                }
            }
            close(fd1[0]);
            close(fd2[1]);
        }
    }else{
        // первый сын
		close(fd1[0]);
		close(fd2[1]);
        FILE * wr = fdopen(fd1[1], "w");
        FILE * rr = fdopen(fd2[0], "r");
        fprintf(wr, "%d", num);
        fflush(wr);
        while(num < n) {
            fscanf(rr, "%d", &num);
            printf("%d %d\n", num, getpid());
            fflush(stdout);
            num++;
            fprintf(wr, "%d ", num);
            fflush(wr);
            if(num >= n){
                break;
            }
        }
        close(fd1[1]);
        close(fd2[0]);
    }
    
    return 0;
}
