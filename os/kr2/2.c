#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd[2];
    if(pipe(fd) == -1){
        return 1;
    }
    char *file1 = argv[1];
    char *file2 = argv[2];
    
    int syn_pip[2];
    int syn_pip2[2];
    if(pipe(syn_pip) == -1 || pipe(syn_pip2)){
        return 1;
    }
    
    pid_t pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    if(!pid1){
        FILE *f = fopen(file1, "r");
        int sum = 0;
        if(f){
            int a;
            while(fscanf(f, "%d", &a) == 1) {
                sum += a;
            }
            
            fclose(f);
        }
        
        fflush(stdout);
        ////////////////////////////////////////
        FILE *syn = fdopen(syn_pip[1], "w");
        FILE *syn2 = fdopen(syn_pip2[0], "r");
        setbuf(syn, NULL);
        setbuf(syn2, NULL);
        fprintf(syn, "1 ");
        
        int b;
        if(fscanf(syn2, "%d", &b) == 1 && b == 2){
            FILE *pfd = fdopen(fd[1], "w");
            setbuf(pfd, NULL);
            fprintf(pfd, "%d ", sum);
            fclose(pfd);
        }
        fclose(syn);
        fclose(syn2);
        /////////////////////////////////////////////
        
        return 0;
    }
    
    pid_t pid2 = fork();
    if(pid2 == -1){
        return 1;
    }
    if(!pid2){
        FILE *f = fopen(file2, "r");
        int sum = 0;
        if(f){
            int a;
            while(fscanf(f, "%d", &a) == 1) {
                sum += a;
            }
            fclose(f);
        }
        fflush(stdout);
        
        ///////////////////////////////////////////////
        FILE *syn2 = fdopen(syn_pip[0], "r");
        setbuf(syn2, NULL);
        int b;
        fflush(stdout);
        if(fscanf(syn2, "%d", &b) == 1 && b == 1){
            fclose(syn2);
            FILE *pfd = fdopen(fd[1], "w");
            setbuf(pfd, NULL);
            fprintf(pfd, "%d ", sum);
            fclose(pfd);
        }
        FILE *syn = fdopen(syn_pip2[1], "w");
        setbuf(syn, NULL);
        fprintf(syn, "2 ");
        
        fclose(syn);
        ////////////////////////////////////////
        
        
        return 0;
    }
    
    close(syn_pip[0]);
    close(syn_pip[1]);
    close(syn_pip2[0]);
    close(syn_pip2[1]);
    close(fd[1]);
    
    int sum1, sum2;
    FILE *prd = fdopen(fd[0], "r");
    fscanf(prd, "%d", &sum1);
    fscanf(prd, "%d", &sum2);
    fclose(prd);
    
    printf("%d\n%d\n%d\n", sum1, sum2, sum1 + sum2);
    fflush(stdout);
    
    return 0;
}
