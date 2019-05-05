#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>


enum
{
    ARGNUM = 2
};

int main(int argc, char *argv[])
{
    if(argc < ARGNUM){
        return 1;
    }
    int N;
    sscanf(argv[1], "%d", &N);
    int fd1[2];
    int fd2[2];
    if(pipe(fd1) == -1 || pipe(fd2) == -1){
        return 1;
    }
    
    pid_t pid1 = fork();
    if(pid1 == -1) {
        return 1;
    }
    if(!pid1){
        // first son
        close(fd2[1]);
        close(fd1[0]);
        
        FILE *f_wr = fdopen(fd1[1], "w");
        FILE *f_rd = fdopen(fd2[0], "r");
        setbuf(f_wr, NULL);
        setbuf(f_rd, NULL);
        
        int a = 1;
        while(fscanf(f_rd, "%d", &a) == 1) {
            if(N == 1) {
                break;
            }
            printf("1 %d\n", a);
            fflush(stdout);
            a++;
            if(a >= N) {
                break;
            }
            fprintf(f_wr, "%d ", a);
        }
        fclose(f_wr);
        fclose(f_rd);
        return 0;
    }
    
    pid_t pid2 = fork();
    if(pid2 == -1){
        return 1;
    }
    if(!pid2) {
        // second son
        close(fd1[1]);
        close(fd2[0]);
        
        FILE *f_rd = fdopen(fd1[0], "r");
        FILE *f_wr = fdopen(fd2[1], "w");
        setbuf(f_wr, NULL);
        setbuf(f_rd, NULL);
        
        int a = 1;
        while(fscanf(f_rd, "%d", &a) == 1) {
            printf("2 %d\n", a);
            fflush(stdout);
            a++;
            if(a >= N) {
                break;
            }
            fprintf(f_wr, "%d ", a);
        }
        fclose(f_wr);
        fclose(f_rd);
        return 0;
    }
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    
    FILE *f_wr = fdopen(fd2[1], "w");
    fprintf(f_wr, "%d ", 1);
    fclose(f_wr);
    
    wait(NULL);
    wait(NULL);
    
    printf("Done\n");
    return 0;
}
