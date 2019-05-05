#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>

volatile int cur_num = 0;

int main(int argc, char *argv[])
{
    int N;
    sscanf(argv[1], "%d", &N);
    unsigned sum = 0;
    for(int i = 2; i < argc; i++){
        cur_num++;
        
        pid_t pid = fork();
        if(pid == -1){
            return 1;
        }
        if(pid > 0){
            
        }else{
            FILE *file = fopen(argv[i], "r");
            char path[PATH_MAX + 2];
            fscanf(file, "%s", path);
            execlp(path, path, NULL);
            _exit(1);
        }
        
        if(cur_num == N) {
            printf("wait this nigga\n");
            fflush(stdout);
            int status;
            while(wait(&status) != -1) {
                if(WIFEXITED(status) && !WEXITSTATUS(status)) {
                    sum++;
                }
            }
            cur_num = 0;
        }
    }
    int status;
    while(wait(&status) != -1) {
        if(WIFEXITED(status) && !WEXITSTATUS(status)) {
            sum++;
        }
    }
    
    printf("%u\n", sum);
    return 0;
}
