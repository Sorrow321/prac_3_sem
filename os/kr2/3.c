#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    char *sep = argv[1];
    int last_start = 2;
    for(int i = 2; i < argc; i++){
        if(!strcmp(sep, argv[i])) {
            
            pid_t pid = fork();
            if(pid == -1){
                return 1;
            }
            if(pid > 0){
                int status;
                wait(&status);
                if(!WIFEXITED(status) || WEXITSTATUS(status)){
                    return 1;
                }
            }else{
                argv[i] = NULL;
                execvp(argv[last_start], &argv[last_start]);
                _exit(1);
            }
            
            if(i != argc - 1){
                last_start = i + 1;
            }else{
                last_start = -1;
            }
        }
    }
    if(argc > 2) {
        if(last_start != -1){
            for(int i = last_start; i < argc; i++){
                argv[i - 1] = argv[i];
            }
            argv[argc - 1] = NULL;
            
            
            pid_t pid = fork();
            if(pid == -1){
                return 1;
            }
            if(pid > 0){
                int status;
                wait(&status);
                if(!WIFEXITED(status) || WEXITSTATUS(status)){
                    return 1;
                }
            }else{
                execvp(argv[last_start - 1], &argv[last_start - 1]);
                _exit(1);
            }
        }
    }
    
    return 0;
}
