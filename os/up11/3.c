#include <unistd.h>
#include <stdio.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>

void wait_sons(int *count){
    while(1){
        int status;
        if(wait(&status) == -1){
            return;
        }
        if(WIFEXITED(status) && !WEXITSTATUS(status)){
            (*count)++;
        }
    }
}

int main(int argc, char *argv[])
{
    int N, count = 0;
    sscanf(argv[1], "%d", &N);
    
    int i;
    for(i = 2; i < argc; i++) {
        if(i < N + 2){
            pid_t pid = fork();
            if(pid == -1){
                return 1;
            }
            if(!pid){
                FILE *f = fopen(argv[i], "r");
                if(f == NULL){
                    _exit(1);
                }
                char cmd[PATH_MAX + 2];
                fscanf(f, "%s", cmd);
                fclose(f);
                
                execlp(cmd, cmd, NULL);
                _exit(1);
            }
        }else{
            wait_sons(&count);
            break;
        }
    }
    wait_sons(&count);
    
    for(; i < argc; i++) {
        pid_t pid = fork();
        if(pid == -1){
            return 1;
        }
        if(pid > 0){
            wait_sons(&count);
        }else{
            FILE *f = fopen(argv[i], "r");
            if(f == NULL){
                _exit(1);
            }
            char cmd[PATH_MAX + 2];
            fgets(cmd, PATH_MAX + 2, f);
            cmd[strlen(cmd) - 1] = 0;
            fclose(f);
            
            execlp(cmd, cmd, NULL);
            _exit(1);
        }
    }
    
    printf("%d\n", count);
    return 0;
}
